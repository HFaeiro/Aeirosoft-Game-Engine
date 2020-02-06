#include "window.h"





window::window(HINSTANCE hInstance, const std::wstring windowName, POINT p)
{
    width = p.x;
    height = p.y;
    hInst = hInstance;
    sWindowName = windowName;
    MyRegisterClass();

    // Perform application initialization:
    if (!InitInstance())
        this->~window();



}

window::~window()
{
    DestroyWindow(myhWnd);
}

std::optional<int> window::processMessages()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return msg.wParam;

        TranslateMessage(&msg);
        DispatchMessage(&msg);

    }

    return {};

}

void window::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = sWindowName.c_str();
    wcex.hIconSm = nullptr;

    RegisterClassExW(&wcex);
}

BOOL window::InitInstance()
{

    myhWnd = CreateWindowW(sWindowName.c_str(), sWindowName.c_str(), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInst, nullptr);

    if (!myhWnd)
    {
        return FALSE;
    }

    SetWindowLongPtr(myhWnd, GWLP_USERDATA, reinterpret_cast<LONG>(this));


    ShowWindow(myhWnd, SW_SHOW);
    UpdateWindow(myhWnd);

    return TRUE;
}

LRESULT window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    window* wthis = reinterpret_cast<window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return wthis->tWndProc(hWnd, message, wParam, lParam);
}

LRESULT window::tWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


