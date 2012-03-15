/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef AABB_H_INCLUDED
#define AABB_H_INCLUDED

#include "DX11VertexBuffer.h"

class AABB : public DX11VertexBuffer
{
public:
  AABB(float x, float y, float z);
};

#endif
