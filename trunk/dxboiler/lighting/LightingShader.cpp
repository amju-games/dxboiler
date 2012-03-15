/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "LightingShader.h"

void LightingShader::UpdateConstantBuffer()
{
  float view[16];
  DX11::GetMatrix(DX11_MODELVIEW_MATRIX, view);
  float proj[16];
  DX11::GetMatrix(DX11_PROJECTION_MATRIX, proj);

  m_cb.mView = XMMatrixTranspose(view); // TODO Why transpose
  m_cb.mProjection = XMMatrixTranspose(proj);

  // Set light colour and dir
  float colour1[] = { 1, 0, 0, 1 };
  float colour2[] = { 0, 1, 0, 1 };
  float dir1[] = { 1, 1, 0, 1 };
  float dir2[] = { -1, -1, 0, 1 };

  m_cb.vLightColor[0] = XMFLOAT4(1, 0, 0, 1);
  m_cb.vLightColor[1] = XMFLOAT4(0, 1, 0, 1);
  m_cb.vLightDir[0] = XMFLOAT4(1, 1, 0, 1);
  m_cb.vLightDir[1] = XMFLOAT4(-1, -1, 0, 1);
}
