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
        // ����Ÿ���� ������ִ����丮��ü�� �����ϰ� �������̽� �����͸����´�.
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    }

    if (SUCCEEDED(hr))
    {
        // �̹� �����찡������� ���¿��� ������ ����� ���Ѵ�.
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
        //        D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE // GDI ȣȯ ��� ����
        //    );

        // ���丮�� �������ڵ�, ����� �Ѱ� ����Ÿ���� �����.
        // ����� ���� ȭ�� �ֻ��� ���� ���� ������ ���, ������ ���� ȭ�� �ֻ��� ������ �޴� ������
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

    // �ؽ�Ʈ
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

        /// ���� ��� ����
        m_pDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
        m_pDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    }

    if (SUCCEEDED(hr))
    {
        // ��Ʈ�� ����� ���� ���丮 ����
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
    // ���ڿ��� ������ �ֿ��� ���ڿ��� ������ �ش� ���Ҹ� ã�´�.
    auto it = std::find_if(m_SharingD2DBitmaps.begin(), m_SharingD2DBitmaps.end(),
        [strFilePath](const std::pair<std::wstring, ID2D1Bitmap*>& ContainerData)
        {
            return (ContainerData.first == strFilePath);
        }
    );

    // ã�� ���
    if (it != m_SharingD2DBitmaps.end())
    {
        *pID2D1Bitmap = it->second;
        (*pID2D1Bitmap)->AddRef();

        return S_OK;
    }

    // Create a decoder - ���ڴ��� �����ؼ� ������ �����ϰ� �޸𸮿� �ø�
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
        // ���丮�� ���� �����͸� �����
        hr = m_pIWICImagingFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        // �����͸� ���� �ȼ� ��ȯ
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

    // �Ҵ� ����
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

    // ���ڿ��� ������ �ֿ��� ���ڿ��� ������ �ش� ���Ҹ� ã�´�.
	const auto it = std::find_if
	(
        m_SharingD2DBitmaps.begin(), m_SharingD2DBitmaps.end(),
	     [pID2D1Bitmap](const std::pair<std::wstring, ID2D1Bitmap*>& ContainerData)
	     {
		     return (ContainerData.second == pID2D1Bitmap);
	     }
	);

    // ã�� ���
    if (it != m_SharingD2DBitmaps.end())
    {
        m_SharingD2DBitmaps.erase(it);
    }
}