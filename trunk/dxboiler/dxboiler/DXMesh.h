/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#ifndef DXMESH_H
#define DXMESH_H

#include <D3D11.h>

class DXMesh
{
public:
	virtual ~DXMesh(){}
	//The draw call
	virtual void Draw(ID3D11DeviceContext* context) = 0;
	//All the necessary steps before draw call
	virtual void PrepareForDrawing(ID3D11DeviceContext* context) = 0;
};

#endif