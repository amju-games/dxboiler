/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef TEXTURING_SHADER_H_INCLUDED
#define TEXTURING_SHADER_H_INCLUDED

#include <DX11Shader.h>

struct ConstantBuffer
{
  XMMATRIX mView;
  XMMATRIX mProjection;

  // TODO What info for texturing ?
	//XMFLOAT4 vLightDir[NUM_LIGHTS];
	//XMFLOAT4 vLightColor[NUM_LIGHTS];
};

class TexturingShader : public DX11ShaderWithCB<ConstantBuffer>
{
public:
  virtual void UpdateConstantBuffer();
//  virtual void UseThisShader(ID3D11DeviceContext* dc);

};

#endif
