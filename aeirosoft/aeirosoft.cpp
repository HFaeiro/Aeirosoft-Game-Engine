
// aeirosoft.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "aeirosoft.h"
#include "app.h"


// Global Variables:


// Forward declarations of functions included in this code module:


int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    POINT p;


    RECT r;
    GetWindowRect(GetDesktopWindow(), &r);
    app a(hInstance, L"Testing", { r.right ,r.bottom });
    return a.begin();


}

