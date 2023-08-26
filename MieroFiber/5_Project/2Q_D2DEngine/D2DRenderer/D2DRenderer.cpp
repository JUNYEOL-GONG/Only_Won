#include "pch.h"
#include "D2DRenderer.h"

#include "../Engine/CommonApp.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

D2DRenderer::~D2DRenderer()
{
    if (m_pD2DRenderTarget)  m_pD2DRenderTarget->Release();
    if (m_pD2DFactory)   m_pD2DFactory->Release();
    if (m_pBrush)       m_pBrush->Release();
    if (m_pDWriteFactory)   m_pDWriteFactory->Release();
    if (m_pDWriteTextFormat) m_pDWriteTextFormat->Release();
    if (m_pIWICImagingFactory)   m_pIWICImagingFactory->Release();

    CoUninitialize();
}

HRESULT D2DRenderer::Initialize()
{
	HRESULT hr = CoInitialize(nullptr);

    if (SUCCEEDED(hr))
    {
        // 렌더타겟을 만들수있는팩토리객체를 생성하고 인터페이스 포인터를얻어온다.
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }

    if (SUCCEEDED(hr))
    {
        // 이미 윈도우가만들어진 상태에서 윈도우 사이즈를 구한다.
        RECT rc;
        GetClientRect(CommonApp::m_hWnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            static_cast<UINT>(rc.right - rc.left),
            static_cast<UINT>(rc.bottom - rc.top)
        );

        //D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
        //        D2D1_RENDER_TARGET_TYPE_DEFAULT,
        //        D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
        //        0,
        //        0,
        //        D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE // GDI 호환 모드 설정
        //    );

        // 팩토리로 윈도우핸들, 사이즈를 넘겨 렌더타겟을 만든다.
        // 디버그 용은 화면 주사율 영향 없이 프레임 출력, 릴리즈 용은 화면 주사율 영향을 받는 프레임
#ifdef _DEBUG
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(CommonApp::m_hWnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
            &m_pD2DRenderTarget
        );
#else
	hr = m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(CommonApp::m_hWnd, size),
			&m_pD2DRenderTarget
		);
#endif
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBrush);
    }

    // 텍스트
    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pDWriteFactory->CreateTextFormat(
            L"Arial",
            nullptr,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20.f,
            L"", //locale
            &m_pDWriteTextFormat
        );
    }

    if (SUCCEEDED(hr))
    {
        // Center the text horizontally and vertically.
        // m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

        /// 완전 가운데 정렬
        m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        m_pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr))
    {
        // 비트맵 출력을 위한 팩토리 생성
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_pIWICImagingFactory)
        );
    }

    if (FAILED(hr))
    {
	    const _com_error err(hr);
        ::MessageBox(CommonApp::m_hWnd, err.ErrorMessage(), L"FAILED", MB_OK);
    }

    return S_OK;
}

HRESULT D2DRenderer::CreateD2DBitmapFromFile(const std::wstring& strFilePath, ID2D1Bitmap** pID2D1Bitmap)
{
    // 문자열과 포인터 쌍에서 문자열만 같으면 해당 원소를 찾는다.
    auto it = std::find_if(m_SharingD2DBitmaps.begin(), m_SharingD2DBitmaps.end(),
        [strFilePath](const std::pair<std::wstring, ID2D1Bitmap*>& ContainerData)
        {
            return (ContainerData.first == strFilePath);
        }
    );

    // 찾은 경우
    if (it != m_SharingD2DBitmaps.end())
    {
        *pID2D1Bitmap = it->second;
        (*pID2D1Bitmap)->AddRef();

        return S_OK;
    }

    // Create a decoder - 디코더를 생성해서 압축을 해제하고 메모리에 올림
    IWICBitmapDecoder* pDecoder = nullptr;

    HRESULT hr = m_pIWICImagingFactory->CreateDecoderFromFilename(
	    strFilePath.c_str(), // Image to be decoded
	    nullptr, // Do not prefer a particular vendor
	    GENERIC_READ, // Desired read access to the file
	    WICDecodeMetadataCacheOnDemand, // Cache metadata when needed
	    &pDecoder // Pointer to the decoder
    );

    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = nullptr;
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    IWICFormatConverter* pConverter = nullptr;
    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        // 팩토리를 통해 컨버터를 만든다
        hr = m_pIWICImagingFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        // 컨버터를 통해 픽셀 변환
        hr = pConverter->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            nullptr,                         // Specify a particular palette
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_pD2DRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, pID2D1Bitmap);
    }

    // 할당 해제
    if (pDecoder != nullptr)
        pDecoder->Release();
    if (pFrame != nullptr)
        pFrame->Release();
    if (pConverter != nullptr)
        pConverter->Release();

    m_SharingD2DBitmaps.emplace_back(strFilePath, *pID2D1Bitmap);

    return hr;
}

void D2DRenderer::ReleaseD2DBitmapFromFile(ID2D1Bitmap* pID2D1Bitmap)
{
	const ULONG count = pID2D1Bitmap->Release();
    if (count > 0)
        return;

    // 문자열과 포인터 쌍에서 문자열만 같으면 해당 원소를 찾는다.
	const auto it = std::find_if
	(
        m_SharingD2DBitmaps.begin(), m_SharingD2DBitmaps.end(),
	     [pID2D1Bitmap](const std::pair<std::wstring, ID2D1Bitmap*>& ContainerData)
	     {
		     return (ContainerData.second == pID2D1Bitmap);
	     }
	);

    // 찾은 경우
    if (it != m_SharingD2DBitmaps.end())
    {
        m_SharingD2DBitmaps.erase(it);
    }
}