/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_SHADER_H_INCLUDED
#define DX11_SHADER_H_INCLUDED

#include <string>
#include <d3d11.h>
#include <d3dx11.h>
#include <D3Dcommon.h>
#define _XM_X86_
#define _XM_NO_INTRINSICS_
#include <xnamath.h>
#include <assert.h>
#include "DX11.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

class DX11Shader 
{
public:
  DX11Shader();
  virtual ~DX11Shader();

  bool Load(const std::string& fxFileName);
  std::string GetErrorString();

  const void* GetBufferPointer();
  size_t GetBufferSize();
  virtual void UseThisShader(ID3D11DeviceContext*);

private:
  std::string m_errorStr;

  ID3D11VertexShader* m_pVertexShader;
  ID3D11PixelShader* m_pPixelShader;
  ID3DBlob* m_pVSBlob; // what is this for ?
};

template <class CONSTANT_BUFFER>
class DX11ShaderWithCB : public DX11Shader
{
public:
  DX11ShaderWithCB()
  {
    m_buf = 0;
    // Create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(CONSTANT_BUFFER);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    HRESULT hr = DX11::GetDevice()->CreateBuffer(&bd, NULL, &m_buf);
    assert(hr == S_OK);
  }

  virtual ~DX11ShaderWithCB()
  {
    if (m_buf) m_buf->Release();
  }

  // Override and set the member variables in your Constant Buffer
  virtual void UpdateConstantBuffer() = 0;

  virtual void UseThisShader(ID3D11DeviceContext* dc)
  {
    DX11Shader::UseThisShader(dc);
    UpdateConstantBuffer();
    dc->UpdateSubresource(m_buf, 0, NULL, &m_cb, 0, 0);
    dc->VSSetConstantBuffers(0, 1, &m_buf);
    dc->PSSetConstantBuffers(0, 1, &m_buf); 
  }

protected:
  CONSTANT_BUFFER m_cb;
  ID3D11Buffer* m_buf;
};

#endif
