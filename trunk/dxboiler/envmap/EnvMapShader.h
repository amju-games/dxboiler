/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2013.
*/

#ifndef ENV_MAP_SHADER_H_INCLUDED
#define ENV_MAP_SHADER_H_INCLUDED

#include <DX11Shader.h>

struct ConstantBuffer
{
  XMMATRIX mView;
  XMMATRIX mProjection;
};

class EnvMapShader : public DX11ShaderWithCB<ConstantBuffer>
{
public:
  virtual void UpdateConstantBuffer();
};

#endif
