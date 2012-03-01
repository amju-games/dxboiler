/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef TEAPOT_H
#define TEAPOT_H

#include "DX11Drawable.h"

class DX11VertexBuffer;

class Teapot
{
public:
  Teapot(float scale = 1.0f);
//  void Draw();
  void Set(DX11VertexBuffer* vb);

private:
  Tris m_tris;
};

#endif  // TEAPOT_H

