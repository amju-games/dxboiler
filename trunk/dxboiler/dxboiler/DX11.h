/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_H_INCLUDED
#define DX11_H_INCLUDED

#include <d3d11.h>
#include <vector>
#include <string>
#include "DX11Texture.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

class DX11Shader;
class DX11Drawable;

enum MatrixMode { 
  DX11_MATRIX_NOTSETYET  = -1, 
  DX11_MODELVIEW_MATRIX  =  0, 
  DX11_PROJECTION_MATRIX =  1, 
  DX11_TEXTURE_MATRIX    =  2};

class DX11 
{
public:
  static ID3D11Device* GetDevice();

  // Call to create window
  static bool CreateWindow();

  // Call at end of main()
  static void CleanUp(); 

  // Call to flip back/front buffers
  static void Flip();

  // Set viewport as screen coords
  static void Viewport(int x, int y, int w, int h);

  // Call at start of drawing every frame.
  // Specify clear colour
  static void InitFrame(float clearR, float clearG, float clearB);

  // Set up projection matrix
  static void SetPerspectiveProjection(float fov, float aspectRatio, 
    float nearDist, float farDist);

  // Set 'camera': give position of eye, target position and Up vector
  static void LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ);

  static void Draw(DX11Drawable* vb); 

  // Set current matrix stack
  static void SetMatrixMode(MatrixMode);

  // Set the current Matrix to the identity
  static void SetIdentity();

  // Push/pop current matrix stack
  static void PushMatrix();
  static void PopMatrix();

  // Translate/Scale/Rotate top of current matrix stack
  static void Translate(float x, float y, float z);
  static void Scale(float x, float y, float z);
  static void RotateX(float degs);
  static void RotateY(float degs);
  static void RotateZ(float degs);

  // Get the current value of the given matrix
  static void GetMatrix(MatrixMode, float result[16]);

  static void MultMatrix(const float matrix[16]);

//  static void PushAttrib(unsigned int attrib);
//  static void PopAttrib();

//  static void Enable(unsigned int flags);
//  static void Disable(unsigned int flags);

  //typedef unsigned int TextureHandle;
  //enum TextureType { AMJU_TEXTURE_REGULAR, AMJU_TEXTURE_SPHERE_MAP };
  //enum TextureDepth { AMJU_RGB, AMJU_RGBA };


  // Call to delete Texture handle
//  static void DestroyTextureHandle(TextureHandle*);

  //static void SetTexture(
  //  TextureHandle*, 
  //  TextureType, 
  //  TextureDepth, 
  //  int width, 
  //  int height, 
  //  unsigned char* data);

  // Call to use a Texture once set up
  static void UseTexture(DX11Texture* tex);

//  static void SetTextureMode(TextureType tt);


  static void UseShader(DX11Shader*);
}; // class 

#endif

