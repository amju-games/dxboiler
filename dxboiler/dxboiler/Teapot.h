/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef TEAPOT_H
#define TEAPOT_H

#include "DX11VertexBuffer.h"

class Teapot : public DX11VertexBuffer
{
public:
  Teapot(float scale = 1.0f);
};

#endif  // TEAPOT_H

