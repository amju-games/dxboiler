/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef SIMPLEST_SHADER_H_INCLUDED
#define SIMPLEST_SHADER_H_INCLUDED

#include <DX11Shader.h>

// Very simple: just mview and proj matrices
struct ConstantBuffer
{
  XMMATRIX mView;
  XMMATRIX mProjection;
};

class SimplestShader : public DX11ShaderWithCB<ConstantBuffer>
{
public:
  virtual void UpdateConstantBuffer();
};

#endif
