/*-----------------------------------------------------
 * Made by: Erio
 * Made on: May 21 2013
-----------------------------------------------------*/

#include "stdafx.h"
#include "CUIWindowEditWater.h"

CUIWindowEditWater::CUIWindowEditWater(CWaterSurface *waterSurface, CDevices *devices, rect<s32> parentSize)
{
	//Setting up pointers
	m_devices = devices;
	m_guiEnv = m_devices->getGUIEnvironment();
	m_waterSurface = waterSurface;

	//Setting up UI pointers
	m_window = NULL;
	m_sinWaveCheckBox = NULL;
	m_sinWaveText = NULL;
	m_refractionCheckBox = NULL;
	m_refractionText = NULL;
	m_drawSceneCheckBox = NULL;
	m_drawSceneText = NULL;

	//Creation of the UI positions
	rect<s32> m_parentSize = parentSize;
	rect<s32> windowSize = rect<s32>(m_parentSize.UpperLeftCorner.X,
									 m_parentSize.UpperLeftCorner.Y + m_parentSize.getHeight(),
									 m_parentSize.UpperLeftCorner.X + (m_parentSize.getWidth()/2),
									 m_parentSize.UpperLeftCorner.Y + m_parentSize.getHeight() + 115);
	rect<s32> sinWaveCheckBoxSize = rect<s32>(10, 10, 35, 35);
	rect<s32> sinWaveTextSize = rect<s32>(45, 10, windowSize.getWidth() - 10, 35);
	rect<s32> refractionCheckBoxSize = rect<s32>(10, 45, 35, 70);
	rect<s32> refractionTextSize = rect<s32>(45, 45, windowSize.getWidth() - 10, 70);
	rect<s32> drawSceneCheckBoxSize = rect<s32>(10, 80, 35, 105);
	rect<s32> drawSceneTextSize = rect<s32>(45, 80, windowSize.getWidth() - 10, 105);

	//Adding the UI to the manager
	m_window = m_guiEnv->addWindow(windowSize);
	m_sinWaveCheckBox = m_guiEnv->addCheckBox(m_waterSurface->isSinWaveEnabled(),sinWaveCheckBoxSize, m_window);
	m_sinWaveText = m_guiEnv->addStaticText(L"Activate sin wave", sinWaveTextSize, false, true, m_window);
	m_refractionCheckBox = m_guiEnv->addCheckBox(m_waterSurface->isRefractionEnabled(), refractionCheckBoxSize, m_window);
	m_refractionText = m_guiEnv->addStaticText(L"Activate refraction", refractionTextSize, false, true, m_window);
	m_drawSceneCheckBox = m_guiEnv->addCheckBox(m_waterSurface->isDrawingScene(), drawSceneCheckBoxSize, m_window);
	m_drawSceneText = m_guiEnv->addStaticText(L"Draw the scene", drawSceneTextSize, false, true, m_window);

	m_devices->getEventReceiver()->AddEventReceiver(this);
}

CUIWindowEditWater::~CUIWindowEditWater()
{
	//Removing the GUI and deleting the pointers
	m_window->remove();
	m_devices->getEventReceiver()->RemoveEventReceiver(this);
	delete m_devices;
	delete m_guiEnv;
	delete m_waterSurface;
}

bool CUIWindowEditWater::OnEvent(const SEvent &event)
{
	//Checkbox event management
	if(event.GUIEvent.Caller == m_sinWaveCheckBox && event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED)
	{
		m_waterSurface->setSinWaveEnabled(m_sinWaveCheckBox->isChecked());
		return true;
	}
	else if(event.GUIEvent.Caller == m_refractionCheckBox && event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED)
	{
		m_waterSurface->setRefractionEnabled(m_refractionCheckBox->isChecked());
		return true;
	}
	else if(event.GUIEvent.Caller == m_drawSceneCheckBox && event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED)
	{
		m_waterSurface->setDrawScene(m_drawSceneCheckBox->isChecked());
		return true;
	}
	else
	{
		return false;
	}
}
