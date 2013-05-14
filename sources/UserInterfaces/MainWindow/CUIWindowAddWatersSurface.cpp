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
			m_devices->createFileOpenDialog(L"Select Base Mesh", 0);
			m_isFileDialogOpen = true;
		}
		else if(event.GUIEvent.Caller == m_acceptButton && event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			CWaterSurface *waterSurface;

			io::path extension, filename;
			extension = ".spkg";
			filename = m_devices->getWorkingDirectory() + "shaders/Materials/water_shader";

			waterSurface = new CWaterSurface(m_devices->getSceneManager(), m_devices->getXEffect()->getScreenQuad().rt[1], m_devices->getSceneManager()->getMesh(m_filePath.c_str()), true, true, m_devices->getWorkingDirectory());
			IAnimatedMeshSceneNode *waterNode = waterSurface->getWaterNode();

			CShaderCallback *callback = new CShaderCallback();

			if(waterNode)
			{
				waterNode->setMaterialFlag(EMF_LIGHTING, false);
				waterNode->setMaterialFlag(EMF_NORMALIZE_NORMALS, false);

				stringw waterSurfaceName = L"#water:";
				waterSurfaceName += m_nameEditBox->getText();
				waterNode->setName(waterSurfaceName.c_str());

				m_devices->getXEffect()->addShadowToNode(waterNode, m_devices->getXEffectFilterType(), ESM_RECEIVE);
				m_devices->getCollisionManager()->setCollisionToAnAnimatedNode(waterNode);

				IFileSystem *fileSystem = m_devices->getDevice()->getFileSystem();
				if (fileSystem->addZipFileArchive(stringc(filename + extension).c_str()))
				{

					stringc vertexLines = m_devices->getCore()->getStringcFromIReadFile("vertex.vbs");
					stringc pixelLines = m_devices->getCore()->getStringcFromIReadFile("pixel.fbs");
					stringc constantsLines = m_devices->getCore()->getStringcFromIReadFile("constants.cbs");

					callback->setDevice(m_devices->getDevice());

					callback->setVertexShader(vertexLines.c_str());
					callback->setVertexShaderType(EVST_VS_3_0);
					callback->setPixelShader(pixelLines.c_str());
					callback->setPixelShaderType(EPST_PS_3_0);
					callback->setConstants(constantsLines.c_str());
					callback->buildMaterial(m_devices->getVideoDriver());
				}
				else
				{
					m_devices->addErrorDialog("Error Archive", L"Error when opening the archive\nMaybe the archive is corrupt...", EMBF_OK);
				}

				waterNode->setMaterialType((video::E_MATERIAL_TYPE)callback->getMaterial());

				m_devices->getCoreData()->getWaterSurfaces()->push_back(SWaterSurfacesData(waterSurface, callback,filename + extension));

				m_waterSurfacesListBox->addItem(waterSurfaceName.c_str());
			}

			m_isFileDialogOpen = false;
			m_window->remove();
		}
		else if(event.GUIEvent.Caller == m_cancelButton && event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
		{
			m_isFileDialogOpen = false;
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
