/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "DX11Shader.h"
#include "DX11.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include "DX11ShaderFile.h"
#include "ReportError.h"

// From DXSDK tutorial
HRESULT CompileShaderFromFile(const char* fileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut )
{
  HRESULT hr = S_OK;

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
  // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
  // Setting this flag improves the shader debugging experience, but still allows 
  // the shaders to be optimized and to run exactly the way they will run in 
  // the release configuration of this program.
  dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

  ID3DBlob* pErrorBlob;

  // Read text from file
  DX11ShaderFile shFile;
  if (!shFile.OpenRead(fileName))
  {
    ReportError(std::string("Failed to open DX11 shader file ") + fileName);
    return ~S_OK;
  }

  std::string text;
  std::string s;
  const bool NO_TRIM = false;
  while (shFile.GetString(&s))
  {
    text += s;
    text += "\n";
  }

  hr = D3DX11CompileFromMemory(
    text.c_str(), text.size(), 
    fileName, // path
    0, 0, szEntryPoint, szShaderModel, dwShaderFlags, 0, 0, ppBlobOut, &pErrorBlob, 0);

  if (FAILED(hr))
  {
    if (pErrorBlob)
    {
      ReportError((char*)pErrorBlob->GetBufferPointer());
      pErrorBlob->Release();
    }
    return hr;
  }

  if (pErrorBlob) 
  {
    pErrorBlob->Release();
  }

  return S_OK;
}

DX11Shader::DX11Shader()
{
  m_pVertexShader = 0;
  m_pPixelShader = 0;
  m_pVSBlob = 0;
}

DX11Shader::~DX11Shader()
{
  if (m_pVertexShader) m_pVertexShader->Release();
  if (m_pPixelShader) m_pPixelShader->Release();
}

bool DX11Shader::Load(const std::string& fxFileName)
{
  ID3D11Device* dd = DX11::GetDevice();

  // Compile the vertex shader
  HRESULT hr = CompileShaderFromFile(fxFileName.c_str(), "VS", "vs_4_0", &m_pVSBlob );
  if (FAILED(hr))
  {
    return false;
  }

	// Create the vertex shader
	hr = dd->CreateVertexShader(m_pVSBlob->GetBufferPointer(), 
    m_pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{	
    ReportError("Failed to create vertex shader for " + fxFileName);
    m_pVSBlob->Release();
    return false;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
  hr = CompileShaderFromFile(fxFileName.c_str(), "PS", "ps_4_0", &pPSBlob );
	if (FAILED(hr))  
  {
    ReportError("Failed to compile pixel shader for " + fxFileName);
    return false;
  }

	// Create the pixel shader
	hr = dd->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
	pPSBlob->Release();
  if (FAILED(hr))
  {
    return false;
    ReportError("Failed to create pixel shader for " + fxFileName);
  }

  return true;
}

const void* DX11Shader::GetBufferPointer()
{
  return m_pVSBlob->GetBufferPointer();
}

size_t DX11Shader::GetBufferSize()
{
  return m_pVSBlob->GetBufferSize();
}

void DX11Shader::UseThisShader(ID3D11DeviceContext* ddc)
{
	ddc->VSSetShader(m_pVertexShader, NULL, 0);
	ddc->PSSetShader(m_pPixelShader, NULL, 0);

  // TODO in subclasses do other stuff, e.g. for texturing.
}

std::string DX11Shader::GetErrorString()
{
  return m_errorStr;
}

//int DX11Shader::GetNumPasses() const
//{
//  
//
//  return m_numPasses;
//}
//
//void DX11Shader::Begin()
//{
//  
//
//}
//
//void DX11Shader::Begin(const std::string& techniqueName)
//{
//  
//
//}
//
//void DX11Shader::End()
//{
//  
//
//}
//
//void DX11Shader::BeginPass(int pass)
//{
//  
//
//}
//
//void DX11Shader::EndPass()
//{
//  
//
//}
//
//void DX11Shader::Set(const std::string& name, const float matrix[16])
//{
//}
//
//void DX11Shader::Set(const std::string& name, float f)
//{
//}
//
//
