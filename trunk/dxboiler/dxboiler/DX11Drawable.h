/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#ifndef DX11_DRAWABLE_H_INCLUDED
#define DX11_DRAWABLE_H_INCLUDED

#include <d3d11.h> 
#include <vector>

class DX11Shader;

struct Vert
{
  Vert() {}
  Vert(float x, float y, float z, float u, float v, float nx, float ny, float nz) :
    m_x(x), m_y(y), m_z(z), m_nx(nx), m_ny(ny), m_nz(nz), m_u(u), m_v(v)
  {
  }

  // Order for D3D FVF
  float m_x, m_y, m_z, m_nx, m_ny, m_nz, m_u, m_v;
};
typedef std::vector<Vert> Verts;

struct Tri
{
  Vert m_verts[3];
};

typedef std::vector<Tri> Tris;

// Derive from this class to draw things. 
// E.g. to draw a list of lines; a list of tris, etc.
class DX11Drawable
{
public:
  DX11Drawable();
  virtual ~DX11Drawable();
  virtual void Draw(ID3D11DeviceContext*, ID3D11Device*, DX11Shader* shader) = 0;

protected:
  ID3D11InputLayout* m_pVertexLayout;
  ID3D11Buffer* m_pVertexBuffer;
  int m_numVerts;
};

#endif
