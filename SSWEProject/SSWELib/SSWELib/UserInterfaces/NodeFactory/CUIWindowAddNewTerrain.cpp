/*
 *  devices.cpp
 *  MacOSX
 *
 *  Created by Julien Moreau-Mathis on 27/09/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stdafx.h"
#include "CUIWindowAddNewTerrain.h"

CUIWindowAddNewTerrain::CUIWindowAddNewTerrain(CDevices *_devices) {
	devices = _devices;

	//GUI ELEMENTS
	IGUIEnvironment *gui = devices->getGUIEnvironment();

	window = gui->addWindow(rect<s32>(670, 320, 900, 460), true, L"Add New Terrain...", 0, -1);
	devices->getCore()->centerWindow(window, devices->getVideoDriver()->getScreenSize());

	gui->addStaticText(L"Size : ", rect<s32>(10, 30, 60, 50), true, true, window, -1, false);
	sizeeb = gui->addEditBox(L"256", rect<s32>(60, 30, 160, 50), true, window, -1);

	randomHillscb = gui->addCheckBox(false, rect<s32>(10, 60, 160, 80), window, -1, L"Random Hills");

	accept = gui->addButton(rect<s32>(10, 90, 110, 120), window, -1, L"Accept", L"Accept and create the terrain");
	close = gui->addButton(rect<s32>(120, 90, 220, 120), window, -1, L"Close", L"Close");

	//DATAS
	devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowAddNewTerrain::~CUIWindowAddNewTerrain() {

}

double CUIWindowAddNewTerrain::interpolate(double a,double b,double x) {
   double ft = x * 3.1415927;
   double f = (1.0-cos(ft)) * 0.5;
   return a * (1.0-f) + b * f;
};


double CUIWindowAddNewTerrain::findNoise(int seed, double x,double y) {
	int n=(int)x+(int)y*57;
	n=(n<<13)^n;
	int nn=(n*(n*n*seed+19990303*seed)+1376312589)&0x7fffffff;
	return 1.0-((double)nn/1073741824.0);
};

double CUIWindowAddNewTerrain::smoothNoise(int seed, double x,double y) {
	int smooth = 1;
	int cr =  16;
	int si = 8;
	int cn = 4;
	double corners = ( findNoise(seed, x-smooth, y-smooth)+findNoise(seed, x+smooth, y-smooth)+findNoise(seed, x-smooth, y+smooth)+findNoise(seed, x+smooth, y+smooth) ) / cr;
	double sides   = ( findNoise(seed, x-smooth, y) +findNoise(seed, x+smooth, y) +findNoise(seed, x, y-smooth) +findNoise(seed, x, y+smooth) ) / si;
	double center  =  findNoise(seed, x, y) / cn;

    return corners + sides + center;
};

double CUIWindowAddNewTerrain::interpolateNoise(int seed, double x, double y) {
	double integer_X    = int(x);
	double fractional_X = x - integer_X;

	double integer_Y    = int(y);
	double fractional_Y = y - integer_Y;

	double v1 = smoothNoise(seed, integer_X,     integer_Y);
	double v2 = smoothNoise(seed, integer_X + 1, integer_Y);
	double v3 = smoothNoise(seed, integer_X,     integer_Y + 1);
	double v4 = smoothNoise(seed, integer_X + 1, integer_Y + 1);

	double i1 = interpolate(v1 , v2 , fractional_X);
	double i2 = interpolate(v3 , v4 , fractional_X);

	return interpolate(i1 , i2 , fractional_Y);
};

ITexture *CUIWindowAddNewTerrain::noise(bool colored, s32 w, s32 h, double zoom, s32 octaves, f32 persistance, s32 r, s32 g, s32 b) {
	IVideoDriver *driver = devices->getVideoDriver();
	double total = 0;

	IImage* NoiseMap = driver->createImage(ECF_R8G8B8,dimension2d<u32>(h,w));
	u8 * ff=(u8 *)NoiseMap->lock();

	for(int y=0;y<h;y++) // loop through height
	{
		for(int x=0;x<w;x++) // loop through width pixels
		{
			total = 0;

			for(int a=0;a<octaves-1;a++)//This loops trough the octaves.
			{
				double frequency = pow(2.0,a);//This increases the frequency with every loop of the octave.
				double amplitude = pow(persistance,a);//This decreases the amplitude with every loop of the octave.

				total = total + interpolateNoise(amplitude, x * frequency / zoom, y / zoom * frequency) * amplitude; // Noise value including smoothing, interpolation, and other noise functions.
			}
			
			if (colored == 1)
			{
				int r, g, b; // Red, Green, Blue
				bool water = false;

				int height = (int)((total*128.0)+128.0);//Convert noise value to 0-256 values for height value.
				if (height<=0)
				{
					height=0;
					water = true;

					r = 0;
					g = 150;
					b = 195; // dark blueish
				}
				if (height>=50)
				{
					water = true;

					r = 0;
					g = 155;
					b = 200; // light blue
				}
				if (height>=145)
				{
					r = 232;
					g = 223;
					b = 230; // sandy color for shore
				}
				if (height>=150)
				{
					r = 114;
					g = 193;
					b = 101; // grass color
				}
				if (height>=200)
				{
					r = 201;
					g = 193;
					b = 101; // dirt color for highlands
				}
				if (height>=250)
				{
					r = 217;
					g = 227;
					b = 237; // snow covered mountains
				}
				if (height>=255)
				{
					height=255;

					r = 227;
					g = 237;
					b = 247; // same as the last just brighter
				}

				NoiseMap->
					setPixel(x,y,SColor(255,(int)r,(int)g,(int)b),false);
			}
			else // color black and white
			{
				int height = (int)((total*128.0)+128.0);//Convert to 0-256 values.
				if(height>255)
					height=255;
				if(height<0)
					height=0;

				NoiseMap->
					setPixel(x,y,SColor(255,(int)height,(int)height,(int)height),false);
			}
		}
	}

	driver->writeImageToFile(NoiseMap, stringc(devices->getWorkingDirectory() + "newterrain.bmp"));
	ITexture *File = driver->getTexture(stringc(devices->getWorkingDirectory() + "newterrain.bmp").c_str());

	return File;
}

bool CUIWindowAddNewTerrain::OnEvent(const SEvent &event) {

	if (event.EventType == EET_GUI_EVENT) {
		if (event.GUIEvent.EventType == EGDT_WINDOW_CLOSE) {
			if (event.GUIEvent.Caller == window) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
		}

		if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
			if (event.GUIEvent.Caller == close) {
				window->remove();
				devices->getEventReceiver()->RemoveEventReceiver(this);
				delete this;
			}
			if (event.GUIEvent.Caller == accept) {
				s32 size = devices->getCore()->getS32(sizeeb->getText());
				if (size <= 10) {
					devices->addWarningDialog(L"Error", L"Please select a size > 10", EMBF_OK);
				} else {
					u32 goodSize = size;
					IImage* tmpImage = devices->getVideoDriver()->createImage(irr::video::ECF_A8R8G8B8, dimension2du(goodSize, goodSize));

					if (randomHillscb->isChecked()) {
						noise(true, goodSize, goodSize, 1.0, 100, 100, 255, 255, 255);
					} else {
						for (u32 i=0; i < goodSize; i++)
							for (u32 j=0; j < goodSize; j++)
								tmpImage->setPixel(i, j, SColor(0, 0, 0, 0), true);
					}

					devices->getVideoDriver()->writeImageToFile(tmpImage, stringc(devices->getWorkingDirectory() + "newterrain.bmp"));

					ITerrainSceneNode *node = devices->getSceneManager()->addTerrainSceneNode(stringc(devices->getWorkingDirectory() + "newterrain.bmp").c_str(),
																							  0, -1, vector3df(0), vector3df(0), vector3df(1.f, 0.1f, 1.f), 
																							  SColor(255, 255, 255, 255), 1024, ETPS_17, 50, true);

					if (remove(stringc(stringc(devices->getWorkingDirectory()) + stringc("newterrain.bmp")).c_str()) != 0)
						devices->addInformationDialog(L"Error", L"Error when deleting the temporary file... \n Not a problem.", EMBF_OK);

					if (node) {
						devices->getCollisionManager()->setCollisionToAnOctTreeNode(node);

						STerrainHMData tdata(node->getMesh(), node, "custom", ETPS_65);
						devices->getCoreData()->getTerrainsData()->push_back(tdata);

						devices->getXEffect()->addShadowToNode(node, devices->getXEffectFilterType(), ESM_BOTH);

						node->setMaterialFlag(EMF_LIGHTING, false);
						node->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

						node->setName("#map:new_terrain");

						ITexture *randTexture = devices->getVideoDriver()->addTexture("custom", tmpImage);

						window->remove();
						devices->getEventReceiver()->RemoveEventReceiver(this);
						delete this;
					} else {
						devices->addWarningDialog(L"Error", L"Error when creating the terrain", EMBF_OK);
					}
				}
			}
		}
	}

	return false;
}
