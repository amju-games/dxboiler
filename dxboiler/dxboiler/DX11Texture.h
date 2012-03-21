/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_TEXTURE_H_INCLUDED
#define DX11_TEXTURE_H_INCLUDED

#include <d3d11.h>
#include <D3DX11tex.h>
#include <string>

class DX11Texture
{
public:
  DX11Texture();
  ~DX11Texture();

  bool Load(const std::string& filename);
  void UseThisTexture(ID3D11DeviceContext*);

protected:
  ID3D11ShaderResourceView* m_pTextureRV;
  ID3D11SamplerState* m_pSamplerLinear;

};

#endif
