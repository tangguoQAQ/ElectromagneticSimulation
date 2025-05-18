#pragma once

#include <d2d1.h>
#include <dwrite.h>

namespace simulation_app
{
    extern ID2D1Factory* pD2DFactory;
    extern ID2D1HwndRenderTarget* pRenderTarget;
    extern IDWriteFactory* pDWriteFactory;

    extern D2D1_SIZE_U renderTargetSize;

    void initializeD2D();

    void uninitializeD2D();

    void resizeD2D();

    namespace graphics
    {
        extern ID2D1SolidColorBrush* pParticleBrush;
        extern ID2D1SolidColorBrush* pFieldLineBrush;
        extern ID2D1SolidColorBrush* pFieldFillBrush;
        extern IDWriteTextFormat* pDefaultTextFormat;
        
        void initializeGraphics();

        void uninitializeGraphics();
    }
} // namespace demo_app