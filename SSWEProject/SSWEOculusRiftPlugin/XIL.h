/*

	Created the 04/08/2012 By Rocipon Etienne.



	WINDOWS: Rocipon Etienne (Erio).		(almost entirely complete: 90%)

	LINUX: Branville Arthur (Ybalrid).		(Undone: 0%)

	OSX: Derdaele Jérémy (Unknown) (Arcanios).	(Undone: 0%)



	License GPL

*/



#ifndef XIL_HPP

#define XIL_HPP



//Include

#include <iostream>

#if __WIN32__ || WIN32



#include <windows.h>



#elif __APPLE__



//include API Apple (Cocoa?)



#else



//include API UNIX (X1Grabber, or somthing like that)



#endif



//Defines definitions: Made using http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx

#if __WIN32__ || WIN32



#define XIL_LMOUSEBUTTON	VK_LBUTTON

#define XIL_RMOUSEBUTTON	VK_RBUTTON

#define XIL_CANCEL		VK_CANCEL

#define XIL_MMOUSEBUTTON	VK_MBUTTON

#define XIL_X1MOUSEBUTTON	VK_XBUTTON1

#define XIL_X2MOUSEBUTTON	VK_XBUTTON2

#define XIL_BACKSPACE	VK_BACK

#define XIL_TAB		VK_TAB

#define XIL_CLEAR	VK_CLEAR

#define XIL_ENTER	VK_RETURN

#define XIL_SHIFT	VK_SHIFT

#define XIL_CTRL	VK_CONTROL

#define XIL_ALT		VK_MENU

#define XIL_PAUSE	VK_PAUSE

#define XIL_CAPSLOCK	VK_CAPITAL

#define XIL_KANA	VK_KANA

#define XIL_HANGUL	VK_HANGUL

#define XIL_JUNJA	VK_JUNJA

#define XIL_FINAL	VK_FINAL

#define XIL_HANJA	VK_HANJA

#define XIL_KANJI	VK_KANJI

#define XIL_ESCAPE	VK_ESCAPE

#define XIL_CONVERT	VK_CONVERT

#define XIL_NONCONVERT	VK_NONCONVERT

#define XIL_ACCEPT	VK_ACCEPT

#define XIL_MODECHANGE	VK_MODECHANGE

#define XIL_SPACE	VK_SPACE

#define XIL_PAGEUP	VK_PRIOR

#define XIL_PAGEDOWN	VK_NEXT

#define XIL_END		VK_END

#define XIL_HOME	VK_HOME

#define XIL_LEFTARROW	VK_LEFT

#define XIL_UPARROW	VK_UP

#define XIL_RIGHTARROW	VK_RIGHT

#define XIL_DOWNARROW	VK_DOWN

#define XIL_SELECT	VK_SELECT

#define XIL_PRINT	VK_PRINT

#define XIL_EXECUTE	VK_EXECUTE

#define XIL_PRINTSCREEN	VK_SNAPSHOT

#define XIL_INSERT	VK_INSERT

#define XIL_DELETE	VK_DELETE

#define XIL_HELP	VK_HELP

#define XIL_0	0x30

#define XIL_1	0x31

#define XIL_2	0x32

#define XIL_3	0x33

#define XIL_4	0x34

#define XIL_5	0x35

#define XIL_6	0x36

#define XIL_7	0x37

#define XIL_8	0x38

#define XIL_9	0x39

#define XIL_A	0x41

#define XIL_B	0x42

#define XIL_C	0x43

#define XIL_D	0x44

#define XIL_E	0x45

#define XIL_F	0x46

#define XIL_G	0x47

#define XIL_H	0x48

#define XIL_I	0x49

#define XIL_J	0x4A

#define XIL_K	0x4B

#define XIL_L	0x4C

#define XIL_M	0x4D

#define XIL_N	0x4E

#define XIL_O	0x4F

#define XIL_P	0x50

#define XIL_Q	0x51

#define XIL_R	0x52

#define XIL_S	0x53

#define XIL_T	0x54

#define XIL_U	0x55

#define XIL_V	0x56

#define XIL_W	0x57

#define XIL_X	0x58

#define XIL_Y	0x59

#define XIL_Z	0x5A

#define XIL_LEFTWINDOWS	VK_LWIN

#define XIL_RIGHTWINDOWS VK_RWIN

#define XIL_APPS	VK_APPS

#define XIL_SLEEP	VK_SLEEP

#define XIL_NUMPAD0	VK_NUMPAD0

#define XIL_NUMPAD1	VK_NUMPAD1

#define XIL_NUMPAD2	VK_NUMPAD2

#define XIL_NUMPAD3	VK_NUMPAD3

#define XIL_NUMPAD4	VK_NUMPAD4

#define XIL_NUMPAD5	VK_NUMPAD5

#define XIL_NUMPAD6	VK_NUMPAD6

#define XIL_NUMPAD7	VK_NUMPAD7

#define XIL_NUMPAD8	VK_NUMPAD8

#define XIL_NUMPAD9	VK_NUMPAD9

#define XIL_MULTIPLY	VK_MULTIPLY

#define XIL_ADDITION	VK_ADD

#define XIL_SEPARATOR	VK_SEPARATOR

#define XIL_SUBSTRACT	VK_SUBTRACT

#define XIL_DECIMAL	VK_DECIMAL

#define XIL_DIVIDE	VK_DIVIDE

#define XIL_F1	VK_F1

#define XIL_F2	VK_F2

#define XIL_F3	VK_F3

#define XIL_F4	VK_F4

#define XIL_F5	VK_F5

#define XIL_F6	VK_F6

#define XIL_F7	VK_F7

#define XIL_F8	VK_F8

#define XIL_F9	VK_F9

#define XIL_F10	VK_F10

#define XIL_F11	VK_F11

#define XIL_F12	VK_F12

#define XIL_F13	VK_F13

#define XIL_F14	VK_F14

#define XIL_F15	VK_F15

#define XIL_F16	VK_F16

#define XIL_F17	VK_F17

#define XIL_F18	VK_F18

#define XIL_F19	VK_F19

#define XIL_F20	VK_F20

#define XIL_F21	VK_F21

#define XIL_F22	VK_F22

#define XIL_F23	VK_F23

#define XIL_F24	VK_F24

#define XIL_NUMLOCK	VK_NUMLOCK

#define XIL_SCROLLLOCK	VK_SCROLL

#define XIL_LSHIFT	VK_LSHIFT

#define XIL_RSHIFT	VK_RSHIFT

#define XIL_LCTRL	VK_LCONTROL

#define XIL_RCTRL	VK_RCONTROL

#define XIL_LMENU	VK_LMENU

#define XIL_RMENU	VK_RMENU

#define XIL_BROWSERBACK	VK_BROWSER_BACK

#define XIL_BROWSERFORWARD	VK_BROWSER_FORWARD

#define XIL_BROWSERREFRESH	VK_BROWSER_REFRESH

#define XIL_BROWSERSTOP	VK_BROWSER_STOP

#define XIL_BROWSERSEARCH	VK_BROWSER_SEARCH

#define XIL_BROWSERFAVOURITES	VK_BROWSER_FAVORITES

#define XIL_BROWSERHOME	VK_BROWSER_HOME

#define XIL_VOLUMEMUTE	VK_VOLUME_MUTE

#define XIL_VOLUMEDOWN	VK_VOLUME_DOWN

#define XIL_VOLUMEUP	VK_VOLUME_UP

#define XIL_MEDIA_NEXTTRACK	VK_MEDIA_NEXT_TRACK

#define XIL_MEDIA_PREVIOUSTRACK	VK_MEDIA_PREV_TRACK

#define XIL_MEDIA_STOP	VK_MEDIA_STOP

#define XIL_MEDIA_PLAYPAUSE	VK_MEDIA_PLAY_PAUSE

#define XIL_LAUNCH_MAIL	VK_LAUNCH_MAIL

#define XIL_LAUNCH_MEDIA_SELECT	VK_LAUNCH_MEDIA_SELECT

#define XIL_LAUNCH_APP1	VK_LAUNCH_APP1

#define XIL_LAUNCH_APP2	VK_LAUNCH_APP2

#define XIL_OEM1	VK_OEM_1

#define XIL_OEM_PLUS	VK_OEM_PLUS

#define XIL_OEM_COMMA	VK_OEM_COMMA

#define XIL_OEM_MINUs	VK_OEM_MINUS

#define XIL_OEM_PERIOD	VK_OEM_PERIOD

#define XIL_OEM2	VK_OEM_2

#define XIL_OEM3	VK_OEM_3

#define XIL_OEM4	VK_OEM_4

#define XIL_OEM5	VK_OEM_5

#define XIL_OEM6	VK_OEM_6

#define XIL_OEM7	VK_OEM_7

#define XIL_OEM8	VK_OEM_8

#define XIL_OEM102	VK_OEM_102

#define XIL_PROCESSKEY	VK_PROCESSKEY

#define XIL_PACKET	VK_PACKET

#define XIL_ATTN	VK_ATTN

#define XIL_CRSEL	VK_CRSEL

#define XIL_EXSEL	VK_EXSEL

#define XIL_EREOF	VK_EREOF

#define XIL_PLAY	VK_PLAY

#define XIL_ZOOM	VK_ZOOM

#define XIL_NONAME	VK_NONAME

#define XIL_PA1		VK_PA1

#define XIL_OEMCLEAR	VK_OEM_CLEAR



#elif __APPLE__



//Mac Defines, please do the closest to windows ones i.e: XIL_A will be valid for windows and OSX



#else



//UNIX defines, please do the closest to windows ones i.e: XIL_A will be valid for windows and Linux



#endif



class XIL

{

	public:

		XIL()

		{

#if __WIN32__ || WIN32

			m_winHnd = 0;

			m_mask = 32768;

#elif __APPLE__

	//Special initializations for OSX may be required, please put them here

#else

	//Special initializations for Linux may be required, please put them here

#endif

		}



		~XIL()

		{



		}



#if __WIN32__ || WIN32



		//It's not necessary to change the Mask for "classical" keyboard

		XIL(HWND winHnd, SHORT mask =32768)

		{

			m_winHnd = winHnd;

			m_mask = mask;

		}



		//using XIL Defined Keys

		//Complete list below for Windows

		bool isPressed(int keyId)

		{

			if(m_winHnd == 0)

			{

				if(GetKeyState(keyId) & m_mask)

					return true;

			}

			else

			{

				if(GetKeyState(keyId) & m_mask && GetForegroundWindow() == m_winHnd)

					return true;

			}



			return false;

		}



		bool isReleased(int keyId)

		{

			if(m_winHnd == 0)

			{

				if(!GetKeyState(keyId) & m_mask)

					return true;

			}

			else

			{

				if(!GetKeyState(keyId) & m_mask && GetForegroundWindow() == m_winHnd)

				return true;

			}



			return false;

		}



		//returns an ptr on array size: 2 containing X and Y positions

		int* getMousePos()

		{

			POINT pos;

			if(GetCursorPos(&pos))

			{

				m_mousePosition[0] = pos.x;

				m_mousePosition[1] = pos.y;

				return m_mousePosition;

			}

			else

				exit(-1);

		}



		//returns true if mouse has been moved successfully

		bool setCursorPos(int x, int y)

		{

			if(!SetCursorPos(x, y))

				return false;

			else

				return true;

		}



#elif __APPLE__



		//returns true if the asked key is pressed

		bool isPressed(/* Here is the keycode defined below: please fit the define names with windows one */)

		{

			return false;

		}



		//returns trus if the asked key isn't pressed

		bool isReleased(/* Here is the keycode defined below: please fit the define names with windows one */)

		{

			return false

		}



		//returns an ptr on array size: 2 containing X and Y positions

		int* getMousePos()

		{



		}



		//returns true if mouse has been moved successfully

		bool setCursorPos(int x, int y)

		{



		}

#else //UNIX



		//returns true if the asked key is pressed

		bool isPressed(/* Here is the keycode defined below: please fit the define names with windows one */)

		{

			return false;

		}



		//returns trus if the asked key isn't pressed

		bool isReleased(/* Here is the keycode defined below: please fit the define names with windows one */)

		{

            return false;

		}



		//returns an ptr on array size: 2 containing X and Y positions

		int* getMousePos()

		{



		}



		//returns true if mouse has been moved successfully

		bool setCursorPos(int x, int y)

		{



		}



#endif



	protected:



	private:

#if __WIN32__ || WIN32

		HWND m_winHnd;

		SHORT m_mask;

		int m_mousePosition[2];

#elif __APPLE__

	//here are the OSX privates attribute of the class to ensure that we're respecting encapsulation rules

#else

	//here are the Linux privates attribute of the class to ensure that we're respecting encapsulation rules

#endif

};



#endif // XIL_HPP

