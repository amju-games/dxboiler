/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "AABB.h"

AABB::AABB(float x, float y, float z)
{
  float xmax = x * 0.5f;
  float ymax = y * 0.5f;
  float zmax = z * 0.5f;

  float xmin = -xmax;
  float ymin = -ymax;
  float zmin = -zmax;

  Tris tris;
  tris.reserve(12);
  Tri t;

  // Bottom
  t.m_verts[0] = Vert(xmin, ymin, zmin, 0, 0,  0, -1, 0);
  t.m_verts[1] = Vert(xmax, ymin, zmin, 1, 0,  0, -1, 0);
  t.m_verts[2] = Vert(xmax, ymin, zmax, 1, 1,  0, -1, 0);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmin, ymin, zmin, 0, 0,  0, -1, 0);
  t.m_verts[1] = Vert(xmax, ymin, zmax, 1, 1,  0, -1, 0);
  t.m_verts[2] = Vert(xmin, ymin, zmax, 0, 1,  0, -1, 0);
  tris.push_back(t);

  // Top
  t.m_verts[0] = Vert(xmax, ymax, zmin, 1, 0,  0, 1, 0);
  t.m_verts[1] = Vert(xmin, ymax, zmin, 0, 0,  0, 1, 0);
  t.m_verts[2] = Vert(xmax, ymax, zmax, 1, 1,  0, 1, 0);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmax, ymax, zmax, 1, 1,  0, 1, 0);
  t.m_verts[1] = Vert(xmin, ymax, zmin, 0, 0,  0, 1, 0);
  t.m_verts[2] = Vert(xmin, ymax, zmax, 0, 1,  0, 1, 0);
  tris.push_back(t);

  // Side
  t.m_verts[0] = Vert(xmax, ymin, zmin, 1, 0,  0, 0, -1);
  t.m_verts[1] = Vert(xmin, ymin, zmin, 0, 0,  0, 0, -1);
  t.m_verts[2] = Vert(xmax, ymax, zmin, 1, 1,  0, 0, -1);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmax, ymax, zmin, 1, 1,  0, 0, -1);
  t.m_verts[1] = Vert(xmin, ymin, zmin, 0, 0,  0, 0, -1);
  t.m_verts[2] = Vert(xmin, ymax, zmin, 0, 1,  0, 0, -1);
  tris.push_back(t);

  // Side
  t.m_verts[0] = Vert(xmin, ymin, zmax, 0, 0,  0, 0, 1);
  t.m_verts[1] = Vert(xmax, ymin, zmax, 1, 0,  0, 0, 1);
  t.m_verts[2] = Vert(xmax, ymax, zmax, 1, 1,  0, 0, 1);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmin, ymin, zmax, 0, 0,  0, 0, 1);
  t.m_verts[1] = Vert(xmax, ymax, zmax, 1, 1,  0, 0, 1);
  t.m_verts[2] = Vert(xmin, ymax, zmax, 0, 1,  0, 0, 1);
  tris.push_back(t);

  // Side
  t.m_verts[0] = Vert(xmax, ymin, zmax, 0, 1,  1, 0, 0);
  t.m_verts[1] = Vert(xmax, ymin, zmin, 0, 0,  1, 0, 0);
  t.m_verts[2] = Vert(xmax, ymax, zmax, 1, 1,  1, 0, 0);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmax, ymax, zmax, 1, 1,  1, 0, 0);
  t.m_verts[1] = Vert(xmax, ymin, zmin, 0, 0,  1, 0, 0);
  t.m_verts[2] = Vert(xmax, ymax, zmin, 1, 0,  1, 0, 0);
  tris.push_back(t);

  // Side
  t.m_verts[0] = Vert(xmin, ymin, zmin, 0, 0,  -1, 0, 0);
  t.m_verts[1] = Vert(xmin, ymin, zmax, 0, 1,  -1, 0, 0);
  t.m_verts[2] = Vert(xmin, ymax, zmax, 1, 1,  -1, 0, 0);
  tris.push_back(t);

  t.m_verts[0] = Vert(xmin, ymin, zmin, 0, 0,  -1, 0, 0);
  t.m_verts[1] = Vert(xmin, ymax, zmax, 1, 1,  -1, 0, 0);
  t.m_verts[2] = Vert(xmin, ymax, zmin, 1, 0,  -1, 0, 0);
  tris.push_back(t);

  SetFromTris(tris);
}


