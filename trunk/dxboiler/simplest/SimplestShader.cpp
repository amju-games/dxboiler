/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "SimplestShader.h"

void SimplestShader::UpdateConstantBuffer()
{
  float view[16];
  DX11::GetMatrix(DX11_MODELVIEW_MATRIX, view);
  float proj[16];
  DX11::GetMatrix(DX11_PROJECTION_MATRIX, proj);

  m_cb.mView = XMMatrixTranspose(view); 
  m_cb.mProjection = XMMatrixTranspose(proj);
}
