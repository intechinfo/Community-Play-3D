//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//
#include "stdafx.h"
#include "CRenderCallback.h"

#include "../../../UserInterfaces/CodeEditor/CUICodeEditor.h"

void CEffectRenderCallback::clearVertexValues() {
    vertexValues.clear();
    vertexValuesNames.clear();
}

void CEffectRenderCallback::clearPixelValues() {
    pixelValues.clear();
    pixelValuesNames.clear();
}

void CEffectRenderCallback::OnPreRender(EffectHandler* effect) {
    for (irr::u32 i=0; i < pixelValuesNames.size(); i++) {
		luaScripting->update();
        luaScripting->runStringScript(pixelValues[i].c_str());
        if (returnedValueType == "integer") {
			float var = returnedValue;
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), &var, returnedValueCount);
		}

        if (returnedValueType == "sampler2D")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), dimension_f, returnedValueCount);
        
        if (returnedValueType == "vec2")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), vector2d_f, returnedValueCount);
        
        if (returnedValueType == "vec3")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), vector3d_f, returnedValueCount);
        
        if (returnedValueType == "mat4")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), worldViewProj.pointer(), returnedValueCount);
    }
}

void CEffectRenderCallback::modifyPixelValue(u32 i, CDevices *devices) { 
	CUICodeEditor *codeEditor = new CUICodeEditor(devices, &pixelValues[i]);
	codeEditor->setAutoSave(true);
}

