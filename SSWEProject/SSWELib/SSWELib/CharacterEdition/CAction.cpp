//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "stdafx.h"
#include "CAction.h"

CAction::CAction() {
    start = 0;
    end = 0;
    name = L"";

    animSpeed = 0;

	animationsPath = L"";
}

CAction::~CAction() {
    
}

irr::core::stringc CAction::getXMLValues() {
	irr::core::stringc toReturn = "";

	toReturn += "<action start=\"";
	toReturn += start;
	toReturn += "\" ";
	toReturn += "end=\"";
	toReturn += end;
	toReturn += "\" name=\"";
	toReturn += name.c_str();
	toReturn += "\" ";
	toReturn += "speed=\"";
	toReturn += animSpeed;
	toReturn += "\" ";
	toReturn += "animPath=\"";
	toReturn += animationsPath;
	toReturn += "\" /> \n";

	return toReturn;
}

void CAction::setXMLValues(irr::io::IrrXMLReader *xmlReader) {
	start = xmlReader->getAttributeValueAsInt("start");
	end = xmlReader->getAttributeValueAsInt("end");
	name = xmlReader->getAttributeValue("name");
	animSpeed = xmlReader->getAttributeValueAsFloat("speed");
	animationsPath = xmlReader->getAttributeValue("animPath");
}
