//
//  CAction.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/12.
//
//

#include "CAction.h"

CAction::CAction() {
    start = 0;
    end = 0;
    name = L"";
    
    animSpeed = 0;
}

CAction::~CAction() {
    
}

stringc CAction::getXMLValues() {
	stringc toReturn = "";

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
	toReturn += "\" /> \n";

	return toReturn;
}
