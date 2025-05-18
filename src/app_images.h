#pragma once

#include <Windows.h>
#include <d2d1.h>

namespace simulation_app
{
    namespace app_images
    {
        void initializeWic();

        void uninitializeWic();
        
        ID2D1Bitmap* load(const wchar_t* filename);

        void unload(ID2D1Bitmap* bitmap);
    } // namespace app_images
    
} // namespace demo_app
