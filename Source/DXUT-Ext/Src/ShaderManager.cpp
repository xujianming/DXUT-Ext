#include "ShaderManager.h"

HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// find the file
	WCHAR str[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, szFileName));

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DX11CompileFromFile(str, NULL, NULL, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}