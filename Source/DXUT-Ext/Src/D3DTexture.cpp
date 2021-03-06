#include "D3DTexture.h"
#include "Core.h"
#include "D3DDriver.h"

// PixelFormat
// 照搬UE的,以后加一种格式都要在这里加上
FPixelFormatInfo GPixelFormats[] =
{
	{ TEXT("unknown"),			0 },
	{ TEXT("A32B32G32R32F"),	0 },
	{ TEXT("A8R8G8B8"),			0 },
	{ TEXT("R8"),				0 },
	{ TEXT("DXT1"),				0 },
	{ TEXT("DXT3"),				0 },
	{ TEXT("DXT5"),				0 },
	{ TEXT("DepthStencil"),		0 },
	{ TEXT("ShadowDepth"),		0 },
};

void FD3D11Driver::InitPixelFormat()
{
	GPixelFormats[PF_Unknown	  ].Format = DXGI_FORMAT_UNKNOWN;
	GPixelFormats[PF_A32B32G32R32F].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GPixelFormats[PF_A8R8G8B8	  ].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	GPixelFormats[PF_R8			  ].Format = DXGI_FORMAT_R8_UNORM;
	GPixelFormats[PF_DXT1		  ].Format = DXGI_FORMAT_BC1_UNORM;
	GPixelFormats[PF_DXT3		  ].Format = DXGI_FORMAT_BC2_UNORM;
	GPixelFormats[PF_DXT5		  ].Format = DXGI_FORMAT_BC3_UNORM;
	GPixelFormats[PF_DepthStencil ].Format = DXGI_FORMAT_R24G8_TYPELESS;
	GPixelFormats[PF_ShadowDepth  ].Format = DXGI_FORMAT_R24G8_TYPELESS;
}

static TSharedPtr<FTexture2D> CreateTexture2DExt(uint Width, uint Height, uint Depth, EPixelFormat PixFormat, bool bTextureArray, bool CubeTexture, uint CreateFlag, uint NumMips)
{
	DXGI_FORMAT DxFmt = (DXGI_FORMAT)GPixelFormats[PixFormat].Format;
	FD3D11Texture2D* pTexture2D = NULL;
	HRESULT hr;

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));
	TextureDesc.Width = Width;
	TextureDesc.Height = Height;
	TextureDesc.MipLevels = NumMips;
	TextureDesc.ArraySize = Depth;
	TextureDesc.Format = DxFmt;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = CubeTexture ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

	//if (Flags & TexCreate_GenerateMipCapable)
	//{
	//	// Set the flag that allows us to call GenerateMips on this texture later
	//	TextureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	//}

	if (CreateFlag & TC_DepthStencil)
	{
		TextureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}
	if (CreateFlag & TC_RenderTarget)
	{
		TextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}

	V(D3D->GetDevice()->CreateTexture2D(
		&TextureDesc,
		NULL,
		&pTexture2D
		));
	TSharedPtr<FD3D11Texture2D> OutTexture2D = MakeCOMPtr<FD3D11Texture2D>(pTexture2D);

	// Create a shader resource view for the texture.
	FD3D11ShaderResourceView* pSRV = NULL;
	TSharedPtr<FD3D11ShaderResourceView> OutSRV;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = TextureDesc.Format;

	if (TextureDesc.Format == DXGI_FORMAT_R24G8_TYPELESS)
	{
		// Use the typed shader resource view format corresponding to DXGI_FORMAT_R24G8_TYPELESS
		SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	}

	SRVDesc.ViewDimension = CubeTexture ? D3D11_SRV_DIMENSION_TEXTURECUBE : (bTextureArray ? D3D_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D);

	if (SRVDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURECUBE)
	{
		SRVDesc.TextureCube.MostDetailedMip = 0;
		SRVDesc.TextureCube.MipLevels = NumMips;
	}
	else if (SRVDesc.ViewDimension == D3D_SRV_DIMENSION_TEXTURE2DARRAY)
	{
		SRVDesc.Texture2DArray.MostDetailedMip = 0;
		SRVDesc.Texture2DArray.MipLevels = NumMips;
		SRVDesc.Texture2DArray.FirstArraySlice = 0;
		SRVDesc.Texture2DArray.ArraySize = TextureDesc.ArraySize;
	}
	else if (SRVDesc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
	{
		SRVDesc.Texture2D.MostDetailedMip = 0;
		SRVDesc.Texture2D.MipLevels = NumMips;
	}

	V(D3D->GetDevice()->CreateShaderResourceView(pTexture2D, &SRVDesc, &pSRV));
	OutSRV = MakeCOMPtr<FD3D11ShaderResourceView>(pSRV);

	return TSharedPtr<FTexture2D>(new FTexture2D(Width, Height, Depth, OutSRV, OutTexture2D));
}

TSharedPtr<FTexture2D> FD3D11Driver::CreateTexture2D(uint Width, uint Height, EPixelFormat PixFormat, uint CreateFlag, uint NumMips)
{
	return CreateTexture2DExt(Width, Height, 1, PixFormat, false, false, CreateFlag, NumMips);
}

