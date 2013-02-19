//
//  enums.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 04/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#include "CCore.h"

CCore::CCore() {
    
}

CCore::~CCore() {
    
}

ISceneNode *CCore::clone(ISceneNode *node, stringc meshPath, ISceneManager *smgr) {
    ISceneNode *clonedNode;
    IMesh *clonedMesh;
    
    clonedMesh = smgr->getMesh(meshPath.c_str());
    clonedNode = smgr->addOctreeSceneNode(clonedMesh);
    
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

s32 CCore::getS32(std::string values32) {
    s32 value_s32;
	stringc value_s32_c = values32.c_str();
	value_s32_c.replace(',', '.');
    
	std::istringstream issX(value_s32_c.c_str());
	issX >> value_s32;
	
	return value_s32;
}

f32 CCore::getF32(std::string valuef32) {
	f32 value_f32;
	stringc value_f32_c = valuef32.c_str();
	value_f32_c.replace(',', '.');
	
	std::istringstream issX(value_f32_c.c_str());
	issX >> value_f32;
	
	return value_f32;
}

u32 CCore::getU32(std::string valueu32) {
	u32 value_u32;
	stringc value_u32_c = valueu32.c_str();
	value_u32_c.replace(',', '.');
	
	std::istringstream issX(value_u32_c.c_str());
	issX >> value_u32;
	
	return value_u32;
}
