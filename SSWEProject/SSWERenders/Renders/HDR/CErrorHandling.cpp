#include "stdafx.h"

#include "CErrorHandling.h"

#ifdef _IRR_WINDOWS_API_
#include <Windows.h>
#endif

void EHandler::ShowErrorDialog(const char* const message, const char* const title) {
    #ifdef _IRR_WINDOWS_API_
	MessageBoxA(nullptr, message, title, MB_ICONERROR | MB_OK);
	#endif
}
