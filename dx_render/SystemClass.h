////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_
// Here we define WIN32_LEAN_AND_MEAN.We do this to speed up the build process, it reduces the size of the Win32 header files by excluding some of the less used APIs.

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN
// Windows.h is included so that we can call the functions to create / destroy windows and be able to use the other useful win32 functions.

//////////////
// INCLUDES //
//////////////
#include <windows.h>
// We have included the headers to the other two classes in the frame work at this point so we can use them in the system class.

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"

// The definition of the class is fairly simple.We see the Initialize, Shutdown, and Run function that was called in WinMain defined here.There are also some private functions that will be called inside those functions.We have also put a MessageHandler function in the class to handle the windows system messages that will get sent to the application while it is running.And finally we have some private variables m_Inputand m_Graphics which will be pointers to the two objects that will handle graphicsand input.

////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(OUT int&,OUT int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;

#endif
