// dx_render.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "dx_render.h"
#include "SystemClass.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    SystemClass* System;
    bool result;


    // Create the system object.
    System = new SystemClass;
    if (!System)
    {
        return 0;
    }

    // Initialize and run the system object.
    result = System->Initialize();
    if (result)
    {
        System->Run();
    }

    // Shutdown and release the system object.
    System->Shutdown();
    delete System;
    System = 0;

    return 0;
}

