// dx11minimal.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "dx11minimal.h"

HWND hWnd;

#include "dx11.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Global Variables for camera:
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;                                       
XMFLOAT3 cameraPosition = XMFLOAT3(-7.0f, 2.5f, -3.0f);
XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
XMFLOAT3 cameraRight = XMFLOAT3(1, 0, 0);
XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);
bool mouseCaptured = true;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DX11MINIMAL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    Dx11Init();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DX11MINIMAL));

    MSG msg = { 0 };

    timer::StartCounter();

    // Main message loop:
    while (msg.message != WM_QUIT)
    {
        double time = timer::GetCounter();

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (time >= timer::nextFrameTime)
        {
            timer::frameBeginTime = timer::GetCounter();
            mainLoop();
            timer::frameEndTime = timer::GetCounter();
            timer::frameRenderingDuration = timer::frameEndTime - timer::frameBeginTime;
            timer::nextFrameTime = timer::frameBeginTime + FRAME_LEN;
        }

        Sleep((DWORD)std::min(FRAME_LEN, max(FRAME_LEN - timer::frameRenderingDuration, 0)));
    }

    return (int) msg.wParam;
}

void UpdateCamera()
{
    // Computing forward vector from yaw and pitch
    cameraForward.x = cos(cameraYaw) * cos(cameraPitch);
    cameraForward.y = sin(cameraPitch);
    cameraForward.z = sin(cameraYaw) * cos(cameraPitch);

    XMVECTOR Forward = XMVector3Normalize(XMLoadFloat3(&cameraForward));
    XMVECTOR Right = XMVector3Normalize(XMVector3Cross(Forward, XMVectorSet(0, 1, 0, 0)));
    XMVECTOR Up = XMVector3Normalize(XMVector3Cross(Right, Forward));

    XMStoreFloat3(&cameraForward, Forward);
    XMStoreFloat3(&cameraRight, Right);
    XMStoreFloat3(&cameraUp, Up);

    XMVECTOR Eye = XMLoadFloat3(&cameraPosition);
    XMVECTOR At = Eye + Forward;

    ConstBuf::camera.view[0] = XMMatrixTranspose(XMMatrixLookAtLH(Eye, At, Up));
    ConstBuf::camera.proj[0] = XMMatrixTranspose(XMMatrixPerspectiveFovLH(DegreesToRadians(90), iaspect, 0.01f, 100.0f));
    ConstBuf::camera.world[0] = XMMatrixIdentity();

    ConstBuf::UpdateCamera();
    ConstBuf::ConstToVertex(3);
    ConstBuf::ConstToPixel(3);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX11MINIMAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DX11MINIMAL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   // Mouse capture
   SetCapture(hWnd);
   ShowCursor(FALSE);
   mouseCaptured = true;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            if (mouseCaptured)
            {
                ReleaseCapture();
                ShowCursor(TRUE);
                mouseCaptured = false;
            }
            else
            {
                SetCapture(hWnd);
                ShowCursor(FALSE);


                RECT rect;
                GetClientRect(hWnd, &rect);
                POINT center;
                center.x = rect.right / 2;
                center.y = rect.bottom / 2;
                ClientToScreen(hWnd, &center);
                SetCursorPos(center.x, center.y);

                mouseCaptured = true;
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
