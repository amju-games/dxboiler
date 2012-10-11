/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#ifndef LOADOBJMESH_H
#define LOADOBJMESH_H

#include "DXMesh.h"
#include <string>

bool LoadObjMesh(DXMesh** MeshInOut, ID3D11DeviceContext* d3dcontext,std::string& filename,bool indexed);

#endif