//
//  CRenderCallback.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 28/11/12.
//
//

#include "CRenderCallback.h"

void CEffectRenderCallback::clearVertexValues() {
    vertexValues.clear();
    vertexValuesNames.clear();
}

void CEffectRenderCallback::clearPixelValues() {
    pixelValues.clear();
    pixelValuesNames.clear();
}

void CEffectRenderCallback::OnPreRender(EffectHandler* effect) {
    luaScripting->update();
    for (irr::u32 i=0; i < pixelValuesNames.size(); i++) {
        luaScripting->runStringScript(pixelValues[i].c_str());
        if (returnedValueType == "integer")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), &returnedValue, returnedValueCount);
        
        if (returnedValueType == "worldViewProj")
            effect->setPostProcessingEffectConstant(materialType, pixelValuesNames[i].c_str(), worldViewProj.pointer(), returnedValueCount);
    }
}
