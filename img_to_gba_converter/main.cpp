#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "resources/bitmap.h"

#define INDEXED

resources::image::BMP img;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
#ifdef INDEXED
    resources::image::load_file("img_indexed.bmp", img);
#else
    resources::image::load_file("img.bmp", img);
#endif
    /*
    img.width = 200;
    img.bytes = img.width * 200;
    std::ifstream imgfile("img.bmp", std::ios::binary);
    while (!imgfile.eof()) {
        COLORREF ref{0};
        for (auto i = 0u; i < 3; ++i) {
            ref |= (imgfile.get() << (16 - i*8));
        }
        img.map.push_back(ref);
    }
    */

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"BMP to GBA converter",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
                SetTextColor(hdc, RGB(255,0,0));

            FillRect(hdc, &ps.rcPaint, (HBRUSH) CreateSolidBrush(COLORREF{RGB(16,16,16)}));
            RECT rect;
            SetRect(&rect, 350, 4, 500, 100);
            wchar_t buffer[256];
            wsprintfW(buffer, L"Type: %c%c(=%d);", (char)(img.type>>8),  (char)(img.type), (signed int)img.type);
            DrawText(hdc, buffer, wcslen(buffer), &rect, DT_TOP|DT_LEFT);
            rect.top += 16;
            rect.bottom += 16;
            wsprintfW(buffer, L"Width: %d;", img.width);
            DrawText(hdc, buffer, wcslen(buffer), &rect, DT_TOP|DT_LEFT);
            rect.top += 16;
            rect.bottom += 16;
            wsprintfW(buffer, L"Height: %d;", img.height);
            DrawText(hdc, buffer, wcslen(buffer), &rect, DT_TOP|DT_LEFT);

            for (auto i = 0u; i < img.map.size(); ++i) {
                SetPixel(hdc, i % img.width, i / img.width, img.map[i]);
                RECT r;
                SetRect(&r, 1.5*(i % img.width), 1.5*(i/img.width), 3*(i%img.width), 3*(i/img.width));
//                FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(img.map[i]));
                rect.top += 16;
                rect.bottom += 16;
                wchar_t buffer[256];
                wsprintfW(buffer, L"(%d, %d) = [%d, %d, %d]", i % img.width, i / img.width, (unsigned char)(img.map[i]>>16), (unsigned char)(img.map[i]>>8), (unsigned char)(img.map[i]));
                DrawText(hdc, buffer, wcslen(buffer), &rect, DT_TOP|DT_LEFT);
            }


            SetRect(&rect,10,10,150,100);
            //       DrawText(HDC, text, text length, drawing area, parameters "DT_XXX")

            EndPaint(hwnd, &ps);
            ReleaseDC(hwnd, hdc);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

