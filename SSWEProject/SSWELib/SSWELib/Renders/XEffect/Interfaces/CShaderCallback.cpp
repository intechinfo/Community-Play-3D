//
//  CShaderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 26/02/13.
//
//
#include "stdafx.h"
#include "CShaderCallback.h"

#include "../../../UserInterfaces/CodeEditor/CUICodeEditor.h"

CShaderCallback::CShaderCallback() {
    material = 0;
    name = "";
    
	constants = "";
    vertexShader = "";
    pixelShader = "";
    
    nullMatrix *= 0;
}

CShaderCallback::~CShaderCallback() {
    
}

CUICodeEditor *CShaderCallback::modifyVertexShader(CDevices *devices) {
	CUICodeEditor *codeEditor = new CUICodeEditor(devices, &vertexShader, true);
	return codeEditor;
}

CUICodeEditor *CShaderCallback::modifyPixelShader(CDevices *devices) {
	CUICodeEditor *codeEditor = new CUICodeEditor(devices, &pixelShader, true);
	return codeEditor;
}

CUICodeEditor *CShaderCallback::modifyConstants(CDevices *devices) {
	CUICodeEditor *codeEditor = new CUICodeEditor(devices, &constants, true);
	return codeEditor;
}

void CShaderCallback::setConstants(irr::core::stringc newConstants) {
	constants = newConstants;
}

void CShaderCallback::buildConstants(irr::video::IVideoDriver *_driver) {
    integers.clear();
    integers_c.clear();
	integers_st.clear();
    
    floats.clear();
    floats_c.clear();
	floats_st.clear();

	vectors2D.clear();
	vectors2D_c.clear();
	vectors2D_st.clear();
    
    vectors3D.clear();
    vectors3D_c.clear();
	vectors3D_st.clear();
    vectors3DAs4Values.clear();
    
    matrixes4.clear();
    matrixes4_c.clear();
	matrixes4_st.clear();
    
    colors.clear();
    colors_c.clear();
	colors_st.clear();
    
    std::string sConstants(stringc(constants).c_str());
    std::istringstream iss(sConstants);
    
    do
    {
        std::string sub;
        iss >> sub;
        
		if (sub == "vvector2df" || sub == "pvector2df") {
			irr::core::vector2df myVector;
            if (sub.c_str()[0] == 'v') {
                vectors2D_st.push_back(EST_VERTEX);
            } else {
                vectors2D_st.push_back(EST_PIXEL);
            }
            iss >> sub;
            vectors2D_c.push_back(sub.c_str());

            float values[2];
            values[0] = std::atof(sub.c_str());
            for (int i=1; i < 2; i++) {
                iss >> sub;
                values[i] = std::atof(sub.c_str());
            }
            myVector.set(values[0], values[1]);
            vectors2D.push_back(myVector);
		}

        if (sub == "vvector3df" || sub == "pvector3df") {
            irr::core::vector3df myVector;
            if (sub.c_str()[0] == 'v') {
                vectors3D_st.push_back(EST_VERTEX);
            } else {
                vectors3D_st.push_back(EST_PIXEL);
            }
            iss >> sub;
            vectors3D_c.push_back(sub.c_str());
            
            iss >> sub;
            if (sub == "camPos") {
                vectors3D.push_back(device->getSceneManager()->getActiveCamera()->getPosition());
                vectors3DAs4Values.push_back(false);
            } else if (sub == "camPos4") {
                vectors3D.push_back(device->getSceneManager()->getActiveCamera()->getPosition());
                vectors3DAs4Values.push_back(true);
            } else {
                vectors3DAs4Values.push_back(false);
                float values[3];
                values[0] = std::atof(sub.c_str());
                for (int i=1; i < 3; i++) {
                    iss >> sub;
                    values[i] = std::atof(sub.c_str());
                }
                myVector.set(values[0], values[1], values[2]);
                vectors3D.push_back(myVector);
            }
        }
        
        if (sub == "vmatrix4" || sub == "pmatrix4") {
            irr::core::matrix4 myMatrix;
            //myMatrix *= 0;
            if (sub.c_str()[0] == 'v') {
                matrixes4_st.push_back(EST_VERTEX);
            } else {
                matrixes4_st.push_back(EST_PIXEL);
            }
            iss >> sub;
            matrixes4_c.push_back(sub.c_str());
			for (irr::u32 matrix_i = 0; matrix_i < 3; matrix_i++) {
				iss >> sub;
				irr::core::matrix4 newMat;
				if (sub == "world[1]") {
					newMat = _driver->getTransform(irr::video::ETS_WORLD);
					newMat.makeInverse();
				} else if (sub == "view[1]") {
					newMat = _driver->getTransform(irr::video::ETS_VIEW);
					newMat.makeInverse();
				} else if (sub == "proj[1]") {
					newMat = _driver->getTransform(irr::video::ETS_PROJECTION);
					newMat.makeInverse();
				} else if (sub == "world[0]") {
					newMat = _driver->getTransform(irr::video::ETS_WORLD);
				} else if (sub == "view[0]") {
					newMat = _driver->getTransform(irr::video::ETS_VIEW);
				} else if (sub == "proj[0]") {
					newMat = _driver->getTransform(irr::video::ETS_PROJECTION);
				} else {
					for (irr::u32 i=0; i < matrixes4_c.size(); i++) {
						if (matrixes4_c[i].c_str() == sub) {
							if (i < matrixes4.size())
								newMat = matrixes4[i];
						}
					}
				}
				if (matrix_i == 0) {
					myMatrix = newMat;
				} else {
					myMatrix *= newMat;
				}
			}
            iss >> sub;
            if (sub == "makeInverse") {
                myMatrix.makeInverse();
            } else if (sub != "dontMakeInverse") {
                for (irr::u32 i=0; i < matrixes4_c.size(); i++) {
                    if (matrixes4_c[i].c_str() == sub) {
						if (i < matrixes4.size())
							myMatrix *= matrixes4[i];
                    }
                }
            }
            matrixes4.push_back(myMatrix);
        }
        
        if (sub == "vfloat" || sub == "pfloat") {
            if (sub.c_str()[0] == 'v') {
                floats_st.push_back(EST_VERTEX);
            } else {
                floats_st.push_back(EST_PIXEL);
            }
            iss >> sub;
            floats_c.push_back(sub.c_str());
            iss >> sub;
            if (sub == "+") {
                iss >> sub;
                if (sub == "ctime") {
                    iss >> sub;
                    if (sub == "realTime") {
                        float value = device->getTimer()->getRealTime();
                        floats.push_back(value);
                    } else {
                        float divide = std::atof(sub.c_str());
                        float value = device->getTimer()->getRealTime() / divide;
                        floats.push_back(value);
                    }
                }
            } else {
                float value = std::atof(sub.c_str());
                floats.push_back(value);
            }
        }
        
        if (sub == "vint" || sub == "pint") {
            if (sub.c_str()[0] == 'v') {
                integers_st.push_back(EST_VERTEX);
            } else {
                integers_st.push_back(EST_PIXEL);
            }
            iss >> sub;
            integers_c.push_back(sub.c_str());
            iss >> sub;
            int value = std::atof(sub.c_str());
            integers.push_back(value);
        }

		if (sub == "vSColori" || sub == "pSColori") {
            irr::video::SColor color;
            if (sub.c_str()[0] == 'v') {
                colors_st.push_back(EST_VERTEX);
            } else {
                colors_st.push_back(EST_PIXEL);
            }
			colors_t.push_back(ECT_SCOLOR);
            iss >> sub;
            colors_c.push_back(sub.c_str());
            iss >> sub;
			color.setAlpha(atof(sub.c_str()));
            iss >> sub;
			color.setRed(atof(sub.c_str()));
            iss >> sub;
			color.setGreen(atof(sub.c_str()));
            iss >> sub;
			color.setBlue(atof(sub.c_str()));
            colors.push_back(color);
        }
        
        if (sub == "vSColor" || sub == "pSColor") {
            irr::video::SColorf color;
            if (sub.c_str()[0] == 'v') {
                colors_st.push_back(EST_VERTEX);
            } else {
                colors_st.push_back(EST_PIXEL);
            }
			colors_t.push_back(ECT_SCOLORF);
            iss >> sub;
            colors_c.push_back(sub.c_str());
            iss >> sub;
            color.a = atof(sub.c_str());
            iss >> sub;
            color.r = atof(sub.c_str());
            iss >> sub;
            color.g = atof(sub.c_str());
            iss >> sub;
            color.b = atof(sub.c_str());
            colors.push_back(color);
        }
        
    } while (iss);
}

void CShaderCallback::buildMaterial(irr::video::IVideoDriver *driver) {
    irr::video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
    material = baseMaterial;
        
    buildConstants(driver);

    if (driver->getDriverType() == irr::video::EDT_OPENGL) {
        material = gpu->addHighLevelShaderMaterial(stringc(vertexShader).c_str(), "main", vertexShaderType,
                                                    stringc(pixelShader).c_str(), "main", pixelShaderType,
                                                    this, baseMaterial);
    } else {
		material = gpu->addHighLevelShaderMaterial(stringc(vertexShader).c_str(), "vertexMain", vertexShaderType,
													stringc(pixelShader).c_str(), "pixelMain", pixelShaderType,
													this, baseMaterial);
    }
}

void CShaderCallback::OnSetConstants(irr::video::IMaterialRendererServices *services, irr::s32 userData) {

	buildConstants(services->getVideoDriver());
    
    for (int i=0; i < integers.size(); i++) {
        if (integers_st[i] == EST_VERTEX) {
            services->setVertexShaderConstant(integers_c[i].c_str(), (int *)&integers[i], 1);
        } else {
            services->setPixelShaderConstant(integers_c[i].c_str(), (int *)&integers[i], 1);
        }
    }
    
    for (int i=0; i < floats.size(); i++) {
        if (floats_st[i] == EST_VERTEX) {
            services->setVertexShaderConstant(floats_c[i].c_str(), (float *)&floats[i], 1);
        } else {
            services->setPixelShaderConstant(floats_c[i].c_str(), (float *)&floats[i], 1);
        }
    }
    
    for (int i=0; i < vectors3D.size(); i++) {
        if (vectors3D_st[i] == EST_VERTEX) {
            if (vectors3DAs4Values[i] == false) {
                services->setVertexShaderConstant(vectors3D_c[i].c_str(), (float*)(&vectors3D[i]), 3);
            } else {
                irr::f32 camPos[4];
                device->getSceneManager()->getActiveCamera()->getAbsolutePosition().getAs4Values(&camPos[0]);
                services->setVertexShaderConstant(vectors3D_c[i].c_str(), &camPos[0], 4);
            }
        } else {
            services->setPixelShaderConstant(vectors3D_c[i].c_str(), (float*)(&vectors3D[i]), 3);
        }
    }

	for (int i=0; i < vectors2D.size(); i++) {
		if (vectors2D_st[i] == EST_VERTEX) {
			services->setVertexShaderConstant(vectors2D_c[i].c_str(), (float *)&vectors2D[i], 2);
        } else {
            services->setPixelShaderConstant(vectors2D_c[i].c_str(), (float *)&vectors2D[i], 2);
        }
	}
    
    for (int i=0; i < matrixes4.size(); i++) {
        if (matrixes4_st[i] == EST_VERTEX) {
            services->setVertexShaderConstant(matrixes4_c[i].c_str(), matrixes4[i].pointer(), 16);
        } else {
            services->setPixelShaderConstant(matrixes4_c[i].c_str(), matrixes4[i].pointer(), 16);
        }
    }
    
    for (int i=0; i < colors.size(); i++) {
        if (colors_st[i] == EST_VERTEX) {
            services->setVertexShaderConstant(colors_c[i].c_str(), (float*)(&colors[i]), 4);
        } else {
            services->setPixelShaderConstant(colors_c[i].c_str(), (float*)(&colors[i]), 4);
        }
    }
}
