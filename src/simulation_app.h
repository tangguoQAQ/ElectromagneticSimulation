#pragma once

#include <Windows.h>

namespace simulation_app
{
    extern HINSTANCE hSimulationAppInstance;
    extern HWND hSimulationAppWindow;

    void run(HINSTANCE hInstance, int nCmdShow);
} // namespace simulation_app