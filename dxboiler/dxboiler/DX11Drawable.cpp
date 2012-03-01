/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "DX11Drawable.h"
#include "DX11Shader.h"

DX11Drawable::DX11Drawable()
{
  m_pVertexLayout = 0;
  m_pVertexBuffer = 0;
  m_numVerts = 0;
}

DX11Drawable::~DX11Drawable()
{
  if (m_pVertexBuffer) m_pVertexBuffer->Release();
  if (m_pVertexLayout) m_pVertexLayout->Release();
}

