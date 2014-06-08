//
//  UIWindowRender.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 16/10/12.
//
//
#include "stdafx.h"
#include "UIWindowRender.h"

CUIWindowRender::CUIWindowRender(CDevices *_devices) {
    devices = _devices;
}

CUIWindowRender::~CUIWindowRender() {

}

void CUIWindowRender::open() {
    renderWindow = devices->getGUIEnvironment()->addWindow(rect<s32>(330, 20, 1000, 610),
                                                     false, L"Render Window", 0, -1);
    renderWindow->getCloseButton()->remove();

	viewPort = devices->getGUIEnvironment()->addImage(rect<s32>(10, 30, 660, 490), renderWindow, -1);
    if (viewPort) {
		viewPort->setImage(devices->getXEffect()->getScreenQuad().rt[1]);
		viewPort->setScaleImage(true);
    } else {
        devices->addErrorDialog(L"Error", L"Error when creating the view port", EMBF_OK);
    }

    saveAs = devices->getGUIEnvironment()->addEditBox(L"render.jpg", rect<s32>(450, 510, 660, 530), true,
                                                      renderWindow, -1);
    devices->getGUIEnvironment()->addStaticText(L"Save as : ", rect<s32>(390, 510, 450, 530), false, false,
                                                renderWindow, -1, false);

    shadowsE = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 500, 110, 520),
                                                         renderWindow, -1, L"Shadows");
    shadersE = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 530, 140, 550),
                                                         renderWindow, -1, L"Render Shaders");
    hdrE = devices->getGUIEnvironment()->addCheckBox(false, rect<s32>(10, 560, 110, 580),
                                                         renderWindow, -1, L"HDR");

    devices->getGUIEnvironment()->addButton(rect<s32>(560, 550, 660, 580), renderWindow,
                                            GUI_WINDOW_EVENTS_RENDER_CLOSE, L"Close", L"Close this window");
    devices->getGUIEnvironment()->addButton(rect<s32>(450, 550, 550, 580), renderWindow,
                                            GUI_WINDOW_EVENTS_RENDER_SAVE, L"Save", L"Save the render (.jpg) to this name");

	devices->getEventReceiver()->AddEventReceiver(this, renderWindow);
}

bool CUIWindowRender::OnEvent(const SEvent &event) {

    if (event.EventType == EET_GUI_EVENT) {
        if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {

            s32 id = event.GUIEvent.Caller->getID();

            switch (id) {
                case GUI_WINDOW_EVENTS_RENDER_CLOSE:
                    renderWindow->remove();
                    devices->getEventReceiver()->RemoveEventReceiver(this);
                    delete this;
                    break;

                case GUI_WINDOW_EVENTS_RENDER_SAVE: {

                    bool shadows, shaders;
                    shadows = devices->isXEffectDrawable();

                    devices->setXEffectDrawable(shadowsE->isChecked());
                    devices->setRenderGUI(false);
                    //devices->getCursor()->setVisible(false);

                    devices->getVideoDriver()->beginScene(true, true, SColor(0x0));
                    devices->updateDevice();
                    devices->getVideoDriver()->endScene();
                    IImage * const screenshot = devices->getVideoDriver()->createScreenShot();
                    if (screenshot) {
                        stringc path = "";
                        path += devices->getWorkingDirectory().c_str();
                        path += "/";
                        path += "Renders/";
                        path += saveAs->getText();
                        path += ".jpg";

                        devices->getVideoDriver()->writeImageToFile(screenshot, path.c_str());

                        //system(path.c_str());
                    } else {
                        devices->addErrorDialog("Take Screenshot Error",
                                                L"Cannot take screenshot\n\n"
                                                L"I Think an intern error...", EMBF_OK);
                    }

                    devices->setXEffectDrawable(shadows);
                    devices->setRenderGUI(true);
                    devices->getCursor()->setVisible(true);

                    renderWindow->remove();
                    devices->getEventReceiver()->RemoveEventReceiver(this);
                    delete this;

                }
                    break;

                default:
                    break;
            }
        }
    }

    return false;
}
