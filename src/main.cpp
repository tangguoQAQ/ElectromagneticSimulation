#include <Windows.h>
#include <winrt/base.h>

#include "simulation_app.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    try
    {
        winrt::init_apartment();
        simulation_app::run(hInstance, nCmdShow);
    }
    catch(winrt::hresult_error const& e)
    {
        winrt::hstring message = e.message();
        OutputDebugString(message.c_str());
        
        return e.code();
    }
    catch(...)
    {
        return -1;
    }

    return 0;
}
