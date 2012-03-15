/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_VERTEX_BUFFER_H_INCLUDED
#define DX11_VERTEX_BUFFER_H_INCLUDED

#include "DX11Drawable.h"

class DX11Shader;

class DX11VertexBuffer : public DX11Drawable
{
public:
  DX11VertexBuffer();

  virtual void Draw(ID3D11DeviceContext*, ID3D11Device*, DX11Shader* shader);
  bool SetFromTris(const Tris& tris);

protected:
  bool m_createdInputLayout;
};

#endif
