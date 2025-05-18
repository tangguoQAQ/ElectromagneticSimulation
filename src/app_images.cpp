#include "app_images.h"

#include <d2d1.h>
#include <wincodec.h>
#include <winrt/base.h>

#include "d2d_resource.h"

namespace simulation_app
{
    namespace app_images
    {
        using winrt::check_hresult;

        winrt::com_ptr<IWICImagingFactory> wicFactory;
        winrt::com_ptr<IWICFormatConverter> convertedSourceBitmap;

        void initializeWic()
        {
            wicFactory = winrt::create_instance<IWICImagingFactory>(CLSID_WICImagingFactory);
        }

        void uninitializeWic()
        {
            wicFactory = nullptr;
            convertedSourceBitmap = nullptr;
        }

        ID2D1Bitmap* load(const wchar_t* filename)
        {
            ID2D1Bitmap* bitmap = nullptr;

            winrt::com_ptr<IWICBitmapDecoder> decoder;
            check_hresult(wicFactory->CreateDecoderFromFilename(
                filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.put()));
            
            winrt::com_ptr<IWICBitmapFrameDecode> frame;
            check_hresult(decoder->GetFrame(0, frame.put()));

            convertedSourceBitmap = nullptr;
            check_hresult(wicFactory->CreateFormatConverter(convertedSourceBitmap.put()));
            check_hresult(convertedSourceBitmap->Initialize(
                frame.get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone,
                nullptr, 0.0f, WICBitmapPaletteTypeCustom));
            
            check_hresult(pRenderTarget->CreateBitmapFromWicBitmap(convertedSourceBitmap.get(), &bitmap));

            return bitmap;
        }

        void unload(ID2D1Bitmap* bitmap)
        {
            if (bitmap)
            {
                bitmap->Release();
            }
        }

    } // namespace app_images
} // namespace demo_app
