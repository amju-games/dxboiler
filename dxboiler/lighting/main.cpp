/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.

LIGHTING
*/

#include <iostream>
#include <DX11.h>
#include <ReportError.h>
#include <Teapot.h>
#include <AABB.h>
#include <DX11VertexBuffer.h>
#include "LightingShader.h"

int main(int argc, char **argv)
{
  std::cout << "Using DX11, woohoo\n";

  if (!DX11::CreateWindow()) 
  {
    ReportError("Failed to create window!?");
    return 1;
  }

  DX11Shader* shader = new LightingShader; 
  // You derive from DX11Shader to do app-specific stuff 
  // Subclasses have vars corresponding to uniform vars in shader
  
  if (!shader->Load("lighting.fx"))
  {
    return 1;
  }

  DX11::UseShader(shader);
  DX11::Viewport(0, 0, 640, 480);

  DX11::SetMatrixMode(DX11_PROJECTION_MATRIX);
  DX11::SetIdentity();
  DX11::SetPerspectiveProjection(45.0f, 1.3f, 1.0f, 1000.0f);

  Teapot tp;
  DX11VertexBuffer vb;
  tp.Set(&vb);

  AABB box(1, 2, 3);

  while (true)
  {
    DX11::InitFrame(0, 0, 1); // clear colour

    DX11::SetMatrixMode(DX11_MODELVIEW_MATRIX);
    DX11::SetIdentity();
    DX11::LookAt(0, 0, 10,  0, 0, 0,  0, 1, 0);

    static float d = 0;
    d += 0.1f;
    DX11::RotateY(d);
    DX11::Draw(&vb);

    DX11::Translate(3, 0, 0);
    DX11::Draw(&box);

    DX11::Flip();
  }

  return 0;
}
