//
//  CExporter.h
//  MacOSX
//
//  Created by Julien Moreau-Mathis on 09/10/12.
//
//

#ifndef __C_UI_EXPORTER_H_INCLUDED__
#define __C_UI_EXPORTER_H_INCLUDED__

#include "../Device/CDevices.h"

class CExporter {
    
public:
    
    CExporter(CDevices *_devices);
    ~CExporter();
    
    void exportScene(stringc file_path);
    
private:
    
    CDevices *devices;
    
};

#endif