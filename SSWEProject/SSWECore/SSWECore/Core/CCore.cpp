//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"
#include "CCore.h"

CCore::CCore() {

}

CCore::~CCore() {

}

void CCore::maximizeWindow(IGUIWindow *window, rect<s32> minRelativePosition) {
	if (window->getRelativePosition().getWidth() != minRelativePosition.getWidth() || window->getRelativePosition().getHeight() != minRelativePosition.getHeight()) {
		window->setRelativePosition(minRelativePosition);
		window->setDraggable(true);
	} else {
		window->setRelativePosition(rect<s32>(0, 75, device->getVideoDriver()->getScreenSize().Width+2, device->getVideoDriver()->getScreenSize().Height-20));
		window->setDraggable(false);
	}
}

void CCore::centerWindow(IGUIWindow *window, dimension2du screenSize) {
	rect<s32> rp = window->getRelativePosition();

	window->setRelativePosition(rect<s32>(screenSize.Width/2-window->getRelativePosition().getWidth()/2,
										  screenSize.Height/2-window->getRelativePosition().getHeight()/2,
										  screenSize.Width/2+window->getRelativePosition().getWidth()/2,
										  screenSize.Height/2+window->getRelativePosition().getHeight()/2));
}

rect<s32> CCore::getScreenCenterRectFromRect(rect<s32> elementRect) {
	dimension2du screenSize = device->getVideoDriver()->getScreenSize();
	return rect<s32>(screenSize.Width/2-elementRect.getWidth()/2,
					 screenSize.Height/2-elementRect.getHeight()/2,
					 screenSize.Width/2+elementRect.getWidth()/2,
					 screenSize.Height/2+elementRect.getHeight()/2);
}

void CCore::deactiveChildrenOfGUIElement(IGUIElement *element, bool visible) {
	core::list<IGUIElement *>::ConstIterator it = element->getChildren().begin();

	for (; it != element->getChildren().end(); ++it) {
		(*it)->setEnabled(visible);
	}
}

bool CCore::elementIsInArrayOfElements(IGUIElement *element, array<IGUIElement *> elements) {
	bool exists = false;
	if (elements.size() == 0) {
		return exists;
	}

	for (u32 i=0; i < elements.size(); i++) {
		if (elements[i] == element) {
			exists = true;
			break;
		}
	}

	return exists;
}

array<IGUIElement *> CCore::getArrayOfAListOfGUIElementChildren(IGUIElement *element) {
	array<IGUIElement *> elements;
	elements.clear();

	core::list<IGUIElement *>::ConstIterator it = element->getChildren().begin();
	for (; it != element->getChildren().end(); ++it) {
		elements.push_back(*it);
		fillArrayOfGUIElementsFromArrayOfGUIElements(&elements, getArrayOfAListOfGUIElementChildren(*it));
	}

	return elements;
}

void CCore::fillArrayOfGUIElementsFromArrayOfGUIElements(array<IGUIElement *> *toFill, array<IGUIElement *> source) {
	for (u32 i=0; i < source.size(); i++) {
		toFill->push_back(source[i]);
	}
}

array<ISceneNode *> *CCore::getArrayOfAListOfNodeChildren(ISceneNode *node) {
	array<ISceneNode *> nodes;

	core::list<ISceneNode *>::ConstIterator it = node->getChildren().begin();
	for (; it != node->getChildren().end(); ++it) {
		nodes.push_back(*it);
	}

	return &nodes;
}

#ifndef _IRR_OSX_PLATFORM_
s32 CCore::nodeExistsInArray(array<ISceneNode *> *nodes, ISceneNode *node) {
	s32 exists = -1;

	if (nodes->size() == 0) {
		return exists;
	}

	for (u32 i=0; i < nodes->size(); i++) {
		if (nodes->operator[](i) == node) {
			exists = i;
			break;
		}
	}

	return exists;
}
#else
s32 CCore::nodeExistsInArray(array<ISceneNode *>& nodes, ISceneNode *node) {
    s32 exists = -1;
    
    if (nodes.size() == 0) {
        return exists;
    }
    
    for (u32 i=0; i < nodes.size(); i++) {
        if (nodes[i] == node) {
            exists = i;
            break;
        }
    }
    
    return exists;
}
#endif

s32 CCore::textureAlreadyExists(stringc name, IVideoDriver *driver) {
	s32 exists = -1;
	for (u32 i=0; i < driver->getTextureCount(); i++) {
		if (driver->getTextureByIndex(i)->getName().getPath() == name) {
			exists = i;
			break;
		}
	}

	return exists;
}

stringw CCore::getTexturePath(ITexture *texture) {
    stringw texturew = L"";
    if (texture) {
        texturew += texture->getName().getPath().c_str();
    } else {
        texturew += "Empty";
    }
    
    return texturew;
}

stringc CCore::getNodeNamePrefix(ISceneNode *node) {
    stringc prefix = "";
    
    if (node) {
        stringc nodeName = node->getName();
        int i=0;
        while (i < nodeName.size() && nodeName[i] != ':') {
            prefix.append(nodeName[i]);
            i++;
        }
    }
    
    return prefix;
}

ISceneNode *CCore::clone(ISceneNode *node, stringc meshPath, ISceneManager *smgr) {
    ISceneNode *clonedNode;
    IMesh *clonedMesh;
    
    clonedMesh = smgr->getMesh(meshPath.c_str());
	if (node->getType() == ESNT_OCTREE)
		clonedNode = smgr->addOctreeSceneNode(clonedMesh);
	else
		clonedNode = smgr->addMeshSceneNode(clonedMesh);
    
    if (clonedNode) {
        clonedNode->setRotation(node->getRotation());
        clonedNode->setScale(node->getScale());
        stringc clonedNodeName = node->getName();
        clonedNodeName += ":cloned";
        clonedNode->setName(clonedNodeName.c_str());
        
        for (int i=0; i < clonedNode->getMaterialCount(); i++) {
            SMaterial material = node->getMaterial(i);
            clonedNode->getMaterial(i) = material;
        }
    }
    
    return clonedNode;
}

stringw CCore::changeTextWithValue(const wchar_t *text, s32 value) {
	stringw returnText = L"";
	returnText += text;
	returnText += value;
	return returnText;
}

std::string CCore::convertToString(stringw _textToConvert) {
	std::string text = stringc(_textToConvert.c_str()).c_str();
	return text;
}

vector3df CCore::getVector3df(stringw X, stringw Y, stringw Z) {
    vector3df position;
    
    stringc Xx, Yy, Zz;
	
	Xx = X.c_str();
	Xx.replace(',', '.');
    
	Yy = Y.c_str();
	Yy.replace(',', '.');
	
	Zz = Z.c_str();
	Zz.replace(',', '.');
	
	std::istringstream issX(Xx.c_str());
	issX >> position.X;
	std::istringstream issY(Yy.c_str());
	issY >> position.Y;
	std::istringstream issZ(Zz.c_str());
	issZ >> position.Z;
	
	return position;
}

vector3df CCore::getVector3df(std::string X, std::string Y, std::string Z) {
	return this->getVector3df(stringw(X.c_str()).c_str(), stringw(Y.c_str()).c_str(), stringw(Z.c_str()).c_str());
}

stringw CCore::getStrNumber(f32 value) {
	stringw numberw = L"";
	numberw += value;
	numberw.replace(L',', L'.');
	
	return numberw;
}

stringw CCore::getStrNumberU32(u32 value) {
    stringw numberw = L"";
    numberw += value;
	
	return numberw;
}

stringw CCore::getStrVector3df(vector3df v) {
	stringw vw = "X :";
	vw += v.X;
	vw += " Y : ";
	vw += v.Y;
	vw += " Z : ";
	vw += v.Z;

	return vw;
}

dimension2d<u32> CCore::getDimensionU32(std::string sizeW, std::string sizeH) {
	dimension2d<u32> size;
	stringc sizeWu, sizeHu;
	
	sizeWu = sizeW.c_str();
	sizeWu.replace(',', '.');
	
	sizeHu = sizeH.c_str();
	sizeHu.replace(',', '.');
	
	std::istringstream issX(sizeWu.c_str());
	issX >> size.Width;
	std::istringstream issY(sizeHu.c_str());
	issY >> size.Height;
	
	return size;
}

dimension2d<f32> CCore::getDimensionF32(std::string sizeW, std::string sizeH) {
	dimension2d<f32> size;
	stringc sizeWf, sizeHf;
	
	sizeWf = sizeW.c_str();
	sizeWf.replace(',', '.');
	
	sizeHf = sizeH.c_str();
	sizeHf.replace(',', '.');
	
	std::istringstream issX(sizeWf.c_str());
	issX >> size.Width;
	std::istringstream issY(sizeHf.c_str());
	issY >> size.Height;
	
	return size;
}

s32 CCore::getS32(stringc values32) {
    s32 value_s32;
	stringc value_s32_c = values32.c_str();
	value_s32_c.replace(',', '.');
    
	std::istringstream issX(value_s32_c.c_str());
	issX >> value_s32;
	
	return value_s32;
}

f32 CCore::getF32(stringc valuef32) {
	f32 value_f32;
	stringc value_f32_c = valuef32.c_str();
	value_f32_c.replace(',', '.');
	
	std::istringstream issX(value_f32_c.c_str());
	issX >> value_f32;
	
	return value_f32;
}

u32 CCore::getU32(stringc valueu32) {
	u32 value_u32;
	stringc value_u32_c = valueu32.c_str();
	value_u32_c.replace(',', '.');
	
	std::istringstream issX(value_u32_c.c_str());
	issX >> value_u32;
	
	return value_u32;
}

u32 CCore::getNumberOfBuildInMaterialTypes() {
	u32 builtInMatTypeNb=0;
	while (irr::video::sBuiltInMaterialTypeNames[builtInMatTypeNb] != 0) {
		builtInMatTypeNb++;
	}

	return builtInMatTypeNb;
}

array<stringc> CCore::getArrayOfBuildInMaterialTypes() {
	array<stringc> builtInMatTypeArray;
	u32 builtInMatTypeNb=0;
	while (irr::video::sBuiltInMaterialTypeNames[builtInMatTypeNb] != 0) {
		builtInMatTypeArray.push_back(irr::video::sBuiltInMaterialTypeNames[builtInMatTypeNb]);
		builtInMatTypeNb++;
	}

	return builtInMatTypeArray;
}

ITexture *CCore::copyTexture(stringc nameOfTexture, ITexture *texture, IVideoDriver *driver) {
	if (texture->isRenderTarget())
		return 0;

	ITexture *tex = driver->addTexture(dimension2du(texture->getOriginalSize().Width, texture->getOriginalSize().Height), 
													nameOfTexture.c_str(), texture->getColorFormat());
	u8* pixelsTex = (u8 *)tex->lock();
	u8 *pixelsTexm = (u8 *)texture->lock();

	for(u32 ui = 0; ui < tex->getOriginalSize().Width * tex->getOriginalSize().Height; ui++) {
		for (u32 uj=0; uj < 3; uj++) {
			*pixelsTex = *pixelsTexm;
			pixelsTex++; pixelsTexm++;
		}
		pixelsTex++; pixelsTexm++;
	}
	tex->unlock();
	texture->unlock();

	return tex;
}

stringc CCore::getStringcFromFile(stringc pathFile) {
	stringc lines = "";

	std::ifstream file(pathFile.c_str());
	if (file.is_open()) {
		while (file.good()) {
			std::string line;
			std::getline(file, line);
			lines += line.c_str();
			lines += "\n";
		}
		file.close();
	}

	return lines;
}

stringc CCore::getStringcFromIReadFile(stringc pathFile) {
	stringc lines = "";
	IFileSystem* filesys = device->getFileSystem();

	if (!filesys->existFile(pathFile.c_str()))
		return lines;

	IReadFile* textfile = filesys->createAndOpenFile(pathFile);

	if (!textfile)
		return lines;

	stringc s;
	while(this->readline(textfile, &s)) {
		lines += s.c_str();
	}

	textfile->drop();

	return lines;
}

bool CCore::readtoken(IReadFile* f, stringc* str) {
	char ch;
	*str = "";
	while (0 != f->read(&ch, 1)) {
		if ((ch == ' ') || (ch == '\t')) {
			*str += ch;
			return true;
		} else {
			*str += ch;
		}
	}

	return false; 
}

bool CCore::readline(IReadFile* f, stringc* str) {
	char ch;
	*str = "";
	while (0 != f->read(&ch, 1)) {
		if (ch == '\n') {
			*str += ch;
			return true;
		} else {
			*str += ch;
		}
	}

	return false;
}
