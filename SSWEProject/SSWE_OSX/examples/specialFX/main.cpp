//
//  main.cpp
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 07/11/2013.
//
//
#include <iostream>

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

IrrlichtDevice *device = 0;
IVideoDriver *driver = 0;
IGUIEnvironment *ui = 0;
ISceneManager *smgr = 0;

IGUIButton *loadImage = 0;
IGUIButton *exportDat = 0;

IGUIImage *image1 = 0;
IGUIImage *image2 = 0;

stringc currentDir;


class CEventReceiver : public IEventReceiver {
public:
    bool OnEvent(const SEvent &event) {
        
        if (event.EventType == EET_GUI_EVENT) {
            if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED) {
                if (event.GUIEvent.Caller == loadImage) {
                    openImage = ui->addFileOpenDialog();
                }
                
                if (event.GUIEvent.Caller == exportDat) {
                    
                }
            }
            
            if (event.GUIEvent.EventType == EGET_FILE_SELECTED) {
                if (event.GUIEvent.Caller == openImage) {
                    ITexture *tex = driver->getTexture(openImage->getFileName());
                    image1->setImage(tex);
                    
                    //IImage *image = driver->createImage(ECF_A8R8G8B8, image1->getImage()->getOriginalSize());
                    IImage *image = driver->createImage(tex, position2di(0, 0), tex->getOriginalSize());
                    
                    for (u32 i=0; i < image->getDimension().Width; i++) {
                        for (u32 j=0; j < image->getDimension().Height; j++ ) {
                            SColor pixel = image->getPixel(i, j);
                            u32 red = pixel.getRed();
                            u32 green = pixel.getGreen();
                            u32 blue = pixel.getBlue();
                            f32 average = 0.3*red + 0.59*green + 0.11*blue;

                            if (average > 255/2) {
                                //average = 255;
                            }
                            
                            image->setPixel(i, j, SColor(255, average, average, average));
                        }
                    }
                    
                    ITexture *tex2 = driver->addTexture("rt", image);
                    image2->setImage(tex2);
                    
                    FILE *export_file = fopen(stringc(currentDir + stringc("/image.dat")).c_str(), "w");
                    fprintf(export_file, "#image.dat\n");
                    
                    for (u32 i=0; i < image->getDimension().Width; i++) {
                        for (u32 j=0; j < image->getDimension().Height; j++ ) {
                            fprintf(export_file, "%d ", 255-image->getPixel(i, j).getRed());
                        }
                        fprintf(export_file, "\n");
                    }
                    
                    fclose(export_file);
                    
                    ui->addMessageBox(L"Information", L"Ton .dat a été exporté");
                }
            }
        }
        
        return false;
    }
    
private:
    IGUIFileOpenDialog *openImage;
};

int main(int argc, char* argv[]) {
    
    CEventReceiver receiver;
    device = createDevice(EDT_OPENGL, dimension2du(1000, 600), 32, false, false, true, &receiver);
    
    currentDir = device->getFileSystem()->getWorkingDirectory();
    
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    ui = device->getGUIEnvironment();
    
    loadImage = ui->addButton(rect<s32>(10, 10, 110, 30), 0, -1, L"Load Image", L"");
    exportDat = ui->addButton(rect<s32>(10, 40, 110, 60), 0, -1, L"Export Dat", L"");
    
    image1 = ui->addImage(rect<s32>(10, 70, driver->getScreenSize().Width/2 - 10, driver->getScreenSize().Height - 10), 0, -1, L"Original Image", false);
    image2 = ui->addImage(rect<s32>(driver->getScreenSize().Width / 2 + 10, 70, driver->getScreenSize().Width - 10, driver->getScreenSize().Height - 10), 0, -1, L"Converted Image", false);
    
    while (device->run()) {
        
        driver->beginScene(true, true, SColor(0x0));
        
        ui->drawAll();
        
        driver->endScene();
    }
    
    return EXIT_SUCCESS;
}
