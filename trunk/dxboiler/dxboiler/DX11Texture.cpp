/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include <assert.h>
#include "DX11Texture.h"
#include "DX11.h"
#include "ReportError.h"

DX11Texture::DX11Texture()
{
  m_pTextureRV = 0;
  m_pSamplerLinear = 0;
}

DX11Texture::~DX11Texture()
{
  if (m_pSamplerLinear) m_pSamplerLinear->Release();
  if (m_pTextureRV) m_pTextureRV->Release();
}

bool DX11Texture::Load(const std::string& filename)
{
  ID3D11Device* dd = DX11::GetDevice();

  HRESULT hr = D3DX11CreateShaderResourceViewFromFileA(
    dd, filename.c_str(), NULL, NULL, &m_pTextureRV, NULL);

  if (FAILED(hr))
  {
    ReportError("Failed to load texture " + filename);
    return false;
  }

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = dd->CreateSamplerState(&sampDesc, &m_pSamplerLinear);
  if (FAILED(hr))
  {
    ReportError("Failed to create sampler for " + filename);
    return false;
  }

  return true;
}

void DX11Texture::UseThisTexture(ID3D11DeviceContext* dc)
{
  assert(m_pTextureRV);
  assert(m_pSamplerLinear);
  dc->PSSetShaderResources(0, 1, &m_pTextureRV);
  dc->PSSetSamplers(0, 1, &m_pSamplerLinear);
}

