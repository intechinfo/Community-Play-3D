driver:setRttTexture("LightScatteringRTT")

position = utils:getScreenCoordinatesFrom3DPosition(driver:getLightPosition(0))
position.y = 1-position.y

filter:setPixelShaderConstantVector2D("lightPositionOnScreen", position)
