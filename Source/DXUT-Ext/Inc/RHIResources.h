#ifndef _RHI_RESOURCES_H_
#define _RHI_RESOURCES_H_

#include "Core.h"

// 前向声明
PRE_DECLARE_CLASS(FVertexDeclaration)
PRE_DECLARE_CLASS(FRenderSurface)
PRE_DECLARE_CLASS(FTexture2D)

// D3D资源
typedef ID3D11SamplerState			FRHISamplerState;
typedef ID3D11RasterizerState		FRHIRasterState;
typedef ID3D11BlendState			FRHIBlendState;
typedef ID3D11InputLayout			FRHIInputLayout;
typedef ID3D11Buffer				FRHIBuffer;
typedef ID3D11DepthStencilView		FRHIDepthStencilView;
typedef ID3D11RenderTargetView		FRHIRenderTargetView;
typedef ID3D11ShaderResourceView	FRHIShaderResourceView;
typedef ID3D11Texture2D				FRHITexture2D;
typedef ID3D11Texture3D				FRHITexture3D;
typedef ID3D11VertexShader			FRHIVertexShader;
typedef ID3D11PixelShader			FRHIPixelShader;
typedef ID3D11DomainShader			FRHIDomainShader;
typedef ID3D11HullShader			FRHIHullShader;
typedef ID3D11GeometryShader		FRHIGeometryShader;
typedef ID3D11ComputeShader			FRHIComputeShader;


struct FRHIBoundShaderState
{
	TSharedPtr<ID3D11InputLayout>		InputLayouts;
	TSharedPtr<ID3D11VertexShader>		VertexShader;
	TSharedPtr<ID3D11PixelShader>		PixelShader;
	TSharedPtr<ID3D11DomainShader>		DomainShader;
	TSharedPtr<ID3D11HullShader>		HullShader;
	TSharedPtr<ID3D11GeometryShader>	GeometryShader;
	TSharedPtr<ID3D11ComputeShader>		ComputeShader;
};

enum ESamplerFilter
{
	SF_Point,
	SF_Bilinear,
	SF_Trilinear,
	SF_AnisotropicPoint,
	SF_AnisotropicLinear,
};

enum ESamplerAddressMode
{
	AM_Warp,
	AM_Clamp,
	AM_Mirror,
	AM_Border,
};

enum ECompareFunction
{
	CF_Never,
	CF_Less,
	CF_Equal,
	CF_LessEqual,
	CF_Greater,
	CF_NotEqual,
	CF_GreaterEqual,
	CF_Always,
};

enum EPrimitiveType
{
	PT_TriangleList,		
	PT_TriangleStrip,
	PT_LineList,
	PT_QuadList,
};

enum ERasterizerFillMode
{
	FM_Wireframe,
	FM_Solid
};

enum ERasterizerCullMode
{
	CM_None,
	CM_CW,
	CM_CCW
};

enum EBlendOperation
{
	BO_Add,
	BO_Subtract,
	BO_Min,
	BO_Max,
	BO_ReverseSubtract,
};

enum EBlendFactor
{
	BF_Zero,
	BF_One,
	BF_SourceColor,
	BF_InverseSourceColor,
	BF_SourceAlpha,
	BF_InverseSourceAlpha,
	BF_DestAlpha,
	BF_InverseDestAlpha,
	BF_DestColor,
	BF_InverseDestColor,
};

enum EBlendColorWriteEnable
{
	CWE_None,
	CWE_Red,
	CWE_Green,
	CWE_Blue,
	CWE_Alpha,
	CWE_All
};

enum EPixelFormat
{
	PF_Unknown,
	PF_A32B32G32R32F,
	PF_A8R8G8B8,
	PF_R8,
	PF_DXT1,
	PF_DXT3,
	PF_DXT5,
	PF_DepthStencil,		
	PF_ShadowDepth,			

	PF_MaxSize
};

// 贴图的像素格式
struct FPixelFormatInfo
{
	const TCHAR*		Name;
	byte				Format;
};

// 创建贴图的用途，默认0是ShaderResource
enum ETexCreate
{
	TC_DepthStencil		= 1 << 0,
	TC_RenderTarget		= 1 << 1,
};

// RenderTarget的创建Flag
enum ERenderSurfaceCreate
{
	RSC_RenderTarget	= 1 << 0,	// 只创建RT
	RSC_DepthStencil	= 1 << 1,	// 只创建DepthStencil
	RSC_All				= RSC_RenderTarget | RSC_DepthStencil // 两者都创建
};

#endif