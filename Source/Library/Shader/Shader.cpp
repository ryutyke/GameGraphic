#include <Shader/Shader.h>


Shader::Shader(_In_ PCWSTR pszFileName, _In_ PCSTR pszEntryPoint, _In_ PCSTR  pszShaderModel)
	: m_pszFileName(pszFileName)
	, m_pszEntryPoint(pszEntryPoint)
	, m_pszShaderModel(pszShaderModel)
{

}

PCWSTR Shader::GetFileName() const
{
	return m_pszFileName;
}

HRESULT Shader::compile(_Outptr_ ID3DBlob** ppOutBlob)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined (DEBUG) || defined(_DEBUG)
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ComPtr<ID3DBlob> errorBlob;
    hr = D3DCompileFromFile(m_pszFileName, nullptr, nullptr, m_pszEntryPoint, m_pszShaderModel,
        dwShaderFlags, 0, ppOutBlob, errorBlob.GetAddressOf());

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
        }
        return hr;
    }

    return S_OK;
}