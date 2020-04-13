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
    DestroyWindow(getHWND());
}

void window::updateTitle(std::wstring title)
{
    SetWindowText(this->myhWnd, title.c_str());
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
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = sWindowName.c_str();
    wcex.hIconSm = nullptr;

    RegisterClassExW(&wcex);
}

BOOL window::InitInstance()
{

    myhWnd = CreateWindow(sWindowName.c_str(), sWindowName.c_str(), WS_OVERLAPPED,
        CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInst, this);

    if (!myhWnd)
    {
        return FALSE;
    }

    if (SetWindowLongPtr(myhWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this)) == 0)
    {
        int er = GetLastError();
        if (er)
            return false;
    }


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
    window* wthis = reinterpret_cast<window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch (message)
    {
    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT && this->active && this->hideMouse) {
            SetCursor(nullptr);
            return true;
        }
            break;
    case WM_SETFOCUS:
        this->active = true;
        break;
    case WM_KILLFOCUS:
        this->active = false;
        break;
    case WM_CLOSE:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE:
    {
        if(wthis)
           wthis->sizeChange = true;
        break;
    }
    
    case WM_LBUTTONUP:
         this->LeftClick = false;
         break;
    case WM_LBUTTONDOWN:
        this->LeftClick = true;
        break;
    case WM_RBUTTONDOWN:
        this->RightClick = true;
        break;
    case WM_RBUTTONUP:
        this->RightClick = false;
        break;
    case WM_MBUTTONDOWN:
        this->MiddleClick = true;
        break;
    case WM_MBUTTONUP:
        this->MiddleClick = false;
        break;
    case WM_MOUSEMOVE:
    {
        p.x = GET_X_LPARAM(lParam);
        p.y = GET_Y_LPARAM(lParam);
        if (this->active && this->hideMouse)
        {
            RECT r = helper::window::GetRect(hWnd);
            POINT P;
            P.y = r.bottom * .5;
            P.x = r.right * .5;
            ClientToScreen(hWnd, &P);
            SetCursorPos(P.x, P.y);
        }
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


