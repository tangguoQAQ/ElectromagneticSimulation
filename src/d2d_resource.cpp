#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <winrt/base.h>

#include "d2d_resource.h"
#include "simulation_app.h"

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

namespace simulation_app
{
    using winrt::check_hresult;

    ID2D1Factory* pD2DFactory{nullptr};
    ID2D1HwndRenderTarget* pRenderTarget{nullptr};
    IDWriteFactory* pDWriteFactory{nullptr};

    D2D1_SIZE_U renderTargetSize{};

    void initializeD2D()
    {
        check_hresult(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pD2DFactory));
        
        RECT clientRect;
        GetClientRect(hSimulationAppWindow, &clientRect);
        renderTargetSize = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

        check_hresult(pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hSimulationAppWindow, renderTargetSize),
            &pRenderTarget
        ));
        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(20, 20));

        check_hresult(DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory)));

        graphics::initializeGraphics();
    }

    void uninitializeD2D()
    {
        SafeRelease(&pRenderTarget);
        SafeRelease(&pD2DFactory);
        SafeRelease(&pDWriteFactory);

        graphics::uninitializeGraphics();
    }

    void resizeD2D()
    {
        RECT clientRect;
        GetClientRect(hSimulationAppWindow, &clientRect);
        renderTargetSize = D2D1::SizeU(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

        check_hresult(pRenderTarget->Resize(renderTargetSize));
    }

    namespace graphics
    {
        ID2D1SolidColorBrush* pParticleSymbolBrush{nullptr};
        ID2D1SolidColorBrush* pParticleFillBrush{nullptr};
        ID2D1SolidColorBrush* pFieldLineBrush{nullptr};
        ID2D1SolidColorBrush* pFieldFillBrush{nullptr};
        IDWriteTextFormat* pDefaultTextFormat{nullptr};

        void initializeGraphics()
        {
            check_hresult(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pParticleSymbolBrush));
            check_hresult(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &pParticleFillBrush));
            check_hresult(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkSlateGray), &pFieldLineBrush));
            check_hresult(pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF(D2D1::ColorF::Green, 0.06f)), &pFieldFillBrush));

            check_hresult(pDWriteFactory->CreateTextFormat(
                L"Microsoft YaHei",
                nullptr,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                72.0f,
                L"zh-cn",
                &pDefaultTextFormat
            ));
        }

        void uninitializeGraphics()
        {
            SafeRelease(&pParticleSymbolBrush);
            SafeRelease(&pParticleFillBrush);
            SafeRelease(&pFieldLineBrush);
            SafeRelease(&pFieldFillBrush);
            SafeRelease(&pDefaultTextFormat);
        }
    } // namespace graphics

} // namespace demo_app
