// Created by Etienne Rocipon on 09/04/13
//
//

#include "CUIWindowAddWatersSurface.h"

CUIWindowAddWaterSurface::CUIWindowAddWaterSurface(CDevices *device, IGUIListBox *waterSurfacesListBox)
{
    m_devices = device;
    m_waterSurfacesListBox = waterSurfacesListBox;
	m_window = NULL;
    m_nameLabel = NULL;
    m_nameEditBox = NULL;
    m_selectButton = NULL;
    m_acceptButton = NULL;
    m_cancelButton = NULL;

	m_filePath = L"";
	m_isFileDialogOpen = false;

    m_devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowAddWaterSurface::~CUIWindowAddWaterSurface()
{

}

void CUIWindowAddWaterSurface::openWindow()
{
    m_window = m_devices->getGUIEnvironment()->addWindow(rect<s32>(450, 100, 750, 240), true, L"Add a new water surface", 0, -1);
    m_nameLabel = m_devices->getGUIEnvironment()->addStaticText(L"Name: (#water: will be added automatically)", rect<s32>(5, 25, 250, 85), false, false, m_window, -1, false);
    m_nameEditBox = m_devices->getGUIEnvironment()->addEditBox(L"New Water Surface", rect<s32>(5, 50, 210, 80), true, m_window, -1);
	m_selectButton = m_devices->getGUIEnvironment()->addButton(rect<s32>(220, 50, 290, 80), m_window, -1, L"Select",L"Select the mesh that will support the water surface");
	m_acceptButton = m_devices->getGUIEnvironment()->addButton(rect<s32>(5, 110, 70, 135), m_window, -1, L"Accept", L"Accept the current selection and create the water surface");
	m_cancelButton = m_devices->getGUIEnvironment()->addButton(rect<s32>(80, 110, 145, 135), m_window, -1, L"Cancel", L"Cancel the current action and close this windows without adding anything");
}

bool CUIWindowAddWaterSurface::OnEvent(const SEvent &event)
{
	if(event.EventType == EET_GUI_EVENT)
	{
		if(event.GUIEvent.Caller == m_selectButton && event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			m_devices->createFileOpenDialog(L"Select base mesh", 0);
			m_isFileDialogOpen = true;
		}
		else if(event.GUIEvent.Caller == m_acceptButton && event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
            if(m_filePath != L"")
			{
                IAnimatedMesh *waterSurfaceMesh = m_devices->getSceneManager()->getMesh(m_filePath.c_str());
                IAnimatedMeshSceneNode *waterSurfaceNode = m_devices->getSceneManager()->addAnimatedMeshSceneNode(waterSurfaceMesh);
                waterSurfaceNode->setAnimationSpeed(0);
                waterSurfaceNode->setFrameLoop(0, 0);

                if(waterSurfaceNode)
                {
                    video::IGPUProgrammingServices *gpu = m_devices->getVideoDriver()->getGPUProgrammingServices();
                    s32 shaderMaterialType = 0;
                    CShaderCallback *callback = new CShaderCallback();


                    waterSurfaceNode->setMaterialFlag(EMF_LIGHTING, false);
                    waterSurfaceNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

                    stringw waterSurfaceName = L"#water:";
                    waterSurfaceName += m_nameEditBox->getText();
                    waterSurfaceNode->setName(waterSurfaceName.c_str());

                    m_devices->getXEffect()->addShadowToNode(waterSurfaceNode, m_devices->getXEffectFilterType());
                    m_devices->getCollisionManager()->setCollisionToAnAnimatedNode(waterSurfaceNode);

                    shaderMaterialType = gpu->addHighLevelShaderMaterialFromFiles(L"shaders/Materials/Water/default.hlsl", "mainVertexShader",EVST_VS_4_0, "shaders/Materials/Water/default.hlsl", "mainPixelShader", EPST_PS_4_0, callback, video::EMT_TRANSPARENT_ADD_COLOR);
                    waterSurfaceNode->setMaterialTexture(0, m_devices->getVideoDriver()->getTexture(L"shaders/Materials/Water/water_101.JPG"));
                    waterSurfaceNode->setMaterialType((video::E_MATERIAL_TYPE)shaderMaterialType);

                    m_devices->getCoreData()->getWaterSurfaces()->push_back(waterSurfaceNode);
                    m_devices->getCoreData()->getWaterSurfacesPath()->push_back(m_filePath);

                    m_waterSurfacesListBox->addItem(waterSurfaceName.c_str());

                    m_window->remove();
                }
			}
			else
			{
                if(m_filePath == L"")
                    m_devices->addWarningDialog(L"Warning", "Please select a mesh to apply the water surface !", EMBF_OK);
                else
                    m_devices->addWarningDialog(L"Warning", "Error when loading the selected mesh \nPlease verify the path or the model integrity\n", EMBF_OK);
			}
		}
		else if(event.GUIEvent.Caller == m_cancelButton && event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			m_window->remove();
		}

		if(event.GUIEvent.EventType == EGET_FILE_SELECTED)
		{
			if(m_isFileDialogOpen)
			{
				IGUIFileOpenDialog *dialog = (IGUIFileOpenDialog *)event.GUIEvent.Caller;
				m_filePath = dialog->getFileName();
				m_isFileDialogOpen = false;
			}
		}
	}

	return false;
}
