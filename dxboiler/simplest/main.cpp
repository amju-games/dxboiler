/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.

SIMPLEST
*/

#include <iostream>
#include "DX11.h"
#include "ReportError.h"
#include "Teapot.h"
#include "DX11VertexBuffer.h"
#include "SimplestShader.h"

int main(int argc, char **argv)
{
  std::cout << "Using DX11, woohoo\n";

  if (!DX11::CreateWindow()) 
  {
    ReportError("Failed to create window!?");
    return 1;
  }

  DX11Shader* shader = new SimplestShader; 
  // You derive from DX11Shader to do app-specific stuff 
  // Subclasses have vars corresponding to uniform vars in shader
  
  if (!shader->Load("simplest.fx"))
  {
    ReportError("Failed to load shader :-(");
    return 1;
  }

  DX11::UseShader(shader);
  DX11::Viewport(0, 0, 640, 480);

  DX11::SetMatrixMode(DX11_PROJECTION_MATRIX);
  DX11::SetIdentity();
  DX11::SetPerspectiveProjection(45.0f, 1.3f, 1.0f, 1000.0f);

  Teapot tp;

  while (true)
  {
    DX11::InitFrame(1, 0, 0); // red clear colour

    DX11::SetMatrixMode(DX11_MODELVIEW_MATRIX);
    DX11::SetIdentity();
    DX11::LookAt(0, 0, 10,  0, 0, 0,  0, 1, 0);

    static float d = 0;
    d += 0.1f;
    DX11::RotateY(d);
    DX11::Draw(&tp);

    DX11::Flip();
  }

  return 0;
}
