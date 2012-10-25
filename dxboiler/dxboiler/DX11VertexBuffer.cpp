/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "DX11VertexBuffer.h"
#include <d3d11.h>
#include <D3Dcommon.h>
#define _XM_X86_
#define _XM_NO_INTRINSICS_
#include <xnamath.h>
#include <assert.h>
#include "DX11Shader.h"

DX11VertexBuffer::DX11VertexBuffer()
{
  m_createdInputLayout = false;
}

bool DX11VertexBuffer::SetFromTris(const Tris& tris)
{
  int numTris = tris.size();
  m_numVerts = numTris * 3;

  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(Vert) * m_numVerts;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));
  InitData.pSysMem = &tris[0]; //vertices;
  HRESULT hr = DX11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
  if (FAILED(hr))
  {
    return false;
  }

  return true;
}

void DX11VertexBuffer::Draw(ID3D11DeviceContext* dc, ID3D11Device* dd, DX11Shader* shader)
{
  UINT stride = sizeof(Vert);
  UINT offset = 0;
  dc->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

  // Set primitive topology
  dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  if (!m_createdInputLayout)
  {
    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
    UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
    HRESULT hr = dd->CreateInputLayout(layout, numElements, shader->GetBufferPointer(),
                                            shader->GetBufferSize(), &m_pVertexLayout);

    if (FAILED(hr))
    {
      assert(0);
      return;
    }
    m_createdInputLayout = true;
  }

  // Set the input layout
  dc->IASetInputLayout(m_pVertexLayout);

  // shader->UseThisShader(dc); // this is done in DX11::Draw

  dc->Draw(m_numVerts, 0);
}

