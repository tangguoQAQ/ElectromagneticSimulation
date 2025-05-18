#include <Windows.h>

#include "simulation_app.h"
#include "d2d_resource.h"
#include "app_images.h"
#include "app_scene.h"

namespace simulation_app
{
    HINSTANCE hSimulationAppInstance{nullptr};
    HWND hSimulationAppWindow{nullptr};

    void registerWindowClass();
    void createWindowAndShow(int nCmdShow);
    void enterMessageLoop();

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void run(HINSTANCE hInstance, int nCmdShow)
    {
        hSimulationAppInstance = hInstance;

        registerWindowClass();
        createWindowAndShow(nCmdShow);
        enterMessageLoop();
    }

    void registerWindowClass()
    {
        WNDCLASS wc{};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hSimulationAppInstance;
        wc.lpszClassName = L"ElectromagneticSimulationAppWnd";

        RegisterClass(&wc);
    }

    void createWindowAndShow(int nCmdShow)
    {
        CreateWindowEx(0, L"ElectromagneticSimulationAppWnd", L"Electromagnetic Simulation App", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            nullptr, nullptr, hSimulationAppInstance, nullptr);
        ShowWindow(hSimulationAppWindow, nCmdShow);
    }

    void enterMessageLoop()
    {
        MSG msg{};
        while(GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    inline namespace windows_message_handler
    {
        void onCreate()
        {
            CoInitialize(nullptr);

            initializeD2D();
            // app_images::initializeWic();
            // scene_manager::initializeScenes();
        }

        void onDestroy()
        {
            uninitializeD2D();
            // scene_manager::uninitializeScenes();
            // app_images::uninitializeWic();

            CoUninitialize();
        }

        void onPaint()
        {
            pRenderTarget->BeginDraw();
            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
            
            renderScene();

            pRenderTarget->EndDraw();
        }

        void onResize()
        {
            resizeD2D();
        }

        void onKeyup(WPARAM wParam, LPARAM lParam)
        {
            switch(wParam)
            {
                case VK_F1:
                    initializeScene();
                    break;
                
                case VK_F2:
                    uninitializeScene();
                    break;
            }
        }
    } // namespace windows_message_handler

    LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
        case WM_NCCREATE:
            hSimulationAppWindow = hWnd;
            return DefWindowProc(hWnd, uMsg, wParam, lParam);

        case WM_CREATE:
            onCreate();
            break;

        case WM_PAINT:
            onPaint();
            break;

        case WM_SIZE:
            onResize();
            break;

        case WM_KEYUP:
            onKeyup(wParam, lParam);
            break;

        case WM_DESTROY:
            onDestroy();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }

        return 0;
    }

} // namespace simulation_app