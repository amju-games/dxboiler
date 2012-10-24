/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef LIGHTING_SHADER_H_INCLUDED
#define LIGHTING_SHADER_H_INCLUDED

#define _XM_NO_INTRINSICS_
#include <DX11Shader.h>

// Mview and proj matrices plus light colour and direction
enum { NUM_LIGHTS = 2 };

// This struct and the number of lights needs to be the same as in the shader!
struct ConstantBuffer
{
  XMMATRIX mView;
  XMMATRIX mProjection;

	XMFLOAT4 vLightDir[NUM_LIGHTS];
	XMFLOAT4 vLightColor[NUM_LIGHTS];
};

class LightingShader : public DX11ShaderWithCB<ConstantBuffer>
{
public:
  virtual void UpdateConstantBuffer();
};

#endif
