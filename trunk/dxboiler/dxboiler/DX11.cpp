/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "DX11.h"
#include <d3d11.h>
#include <D3Dcommon.h>
#define _XM_X86_
#define _XM_NO_INTRINSICS_
#include <xnamath.h>
#include <Windows.h>
#include <D3DX11tex.h> // TODO In shader(s) which support texturing only
#include <stack>
#include <assert.h>
#include "DX11Shader.h"
#include "DX11Drawable.h"

#ifdef CreateWindow
#undef CreateWindow
#endif

WNDPROC m_wndproc;
DX11Shader* m_currentShader;

HINSTANCE               g_hInst = 0;
HWND                    g_hWnd = 0;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = 0;
ID3D11DeviceContext*    g_pImmediateContext = 0;
IDXGISwapChain*         g_pSwapChain = 0;
ID3D11RenderTargetView* g_pRenderTargetView = 0;
//ID3D11Buffer*           g_pConstantBuffer = 0;

XMMATRIX g_View;
XMMATRIX g_Projection;

ID3D11Texture2D*        g_pDepthStencil = 0;
ID3D11DepthStencilView* g_pDepthStencilView = 0;

ID3D11ShaderResourceView*           g_pTextureRV = 0;
ID3D11SamplerState*                 g_pSamplerLinear = 0;

std::stack<XMMATRIX> mtxStack[3];
MatrixMode s_matrixMode = DX11_MATRIX_NOTSETYET;

// Lots of this code is pasted from DXSDK tutorials
float DegToRad(float degs)
{
  return degs * 0.017453292f;
}

//struct ConstantBuffer
//{
//  XMMATRIX mView;
//  XMMATRIX mProjection;
//
//  // TODO
//  //XMFLOAT4 vLightDir[2];
//  //XMFLOAT4 vLightColor[2];
//};

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
  HRESULT hr = S_OK;

  RECT rc;
  GetClientRect( g_hWnd, &rc );
  UINT width = rc.right - rc.left;
  UINT height = rc.bottom - rc.top;

  UINT createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE( driverTypes );

  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  DXGI_SWAP_CHAIN_DESC sd;
  ZeroMemory( &sd, sizeof( sd ) );
  sd.BufferCount = 1;
  sd.BufferDesc.Width = width;
  sd.BufferDesc.Height = height;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = g_hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;

  for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
      D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
    if( SUCCEEDED( hr ) )
      break;
  }
  if( FAILED( hr ) )
    return hr;

  // Create a render target view
  ID3D11Texture2D* pBackBuffer = NULL;
  hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
  if( FAILED( hr ) )
    return hr;

  hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  pBackBuffer->Release();
  if( FAILED( hr ) )
    return hr;

  // Create depth stencil texture
  D3D11_TEXTURE2D_DESC descDepth;
  ZeroMemory( &descDepth, sizeof(descDepth) );
  descDepth.Width = width;
  descDepth.Height = height;
  descDepth.MipLevels = 1;
  descDepth.ArraySize = 1;
  descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  descDepth.SampleDesc.Count = 1;
  descDepth.SampleDesc.Quality = 0;
  descDepth.Usage = D3D11_USAGE_DEFAULT;
  descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  descDepth.CPUAccessFlags = 0;
  descDepth.MiscFlags = 0;
  hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
  if( FAILED( hr ) )
    return hr;

  // Create the depth stencil view
  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  ZeroMemory( &descDSV, sizeof(descDSV) );
  descDSV.Format = descDepth.Format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  descDSV.Texture2D.MipSlice = 0;
  hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView);
  if( FAILED( hr ) )
    return hr;

  // TODO There seems to be something missing here - i.e. what is the depth
  //  test to use, etc. Check out 
  //  http://www.rastertek.com/dx11tut03.html

  g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  g_pImmediateContext->RSSetViewports( 1, &vp );

  //// Create the constant buffer
  //D3D11_BUFFER_DESC bd;
  //ZeroMemory( &bd, sizeof(bd) );
  //bd.Usage = D3D11_USAGE_DEFAULT;
  //bd.ByteWidth = sizeof(ConstantBuffer);
  //bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  //bd.CPUAccessFlags = 0;
  //hr = g_pd3dDevice->CreateBuffer( &bd, NULL, &g_pConstantBuffer );
  //if (FAILED(hr))
  //  return hr;

  // Create the sample state
  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
  if (FAILED(hr))
    return hr;

  return S_OK;
}

void DX11::CleanUp()
{
  if( g_pImmediateContext ) g_pImmediateContext->ClearState();

  if( g_pSamplerLinear ) g_pSamplerLinear->Release();
  if( g_pTextureRV ) g_pTextureRV->Release();
//  if( g_pConstantBuffer ) g_pConstantBuffer->Release();
  if( g_pRenderTargetView ) g_pRenderTargetView->Release();
  if( g_pDepthStencil ) g_pDepthStencil->Release();
  if( g_pDepthStencilView ) g_pDepthStencilView->Release();
  if( g_pSwapChain ) g_pSwapChain->Release();
  if( g_pImmediateContext ) g_pImmediateContext->Release();
  if( g_pd3dDevice ) g_pd3dDevice->Release();
//  delete m_defaultShader;
}

ID3D11Device* DX11::GetDevice()
{
  return g_pd3dDevice;
}

static LRESULT CALLBACK MyDefaultWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hdc;

  switch( message )
  {
  case WM_PAINT:
    hdc = BeginPaint( hWnd, &ps );
    EndPaint( hWnd, &ps );
    break;

  case WM_DESTROY:
    PostQuitMessage( 0 );
    break;

  case WM_CLOSE:
    exit(0); 
    return 0;

  case WM_QUIT:
    exit(0);
    return 0;

  default:
    return DefWindowProc( hWnd, message, wParam, lParam );
  }

  return 0;
}

bool DX11::CreateWindow()
{
  static LPCWSTR AMJU_WINDOW_CLASS_NAME = L"MY_WINDOWS_CLASS";

  // Register class
  WNDCLASSEX wcex;
  wcex.cbSize = sizeof( WNDCLASSEX );
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = MyDefaultWndProc; // TODO you should supply your own
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = g_hInst; 
  wcex.hIcon = 0; 
  wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = AMJU_WINDOW_CLASS_NAME; 
  wcex.hIconSm = 0; 
  if( !RegisterClassEx( &wcex ) )
      return false; 

  HWND hWnd = CreateWindowEx(
    NULL, 
    AMJU_WINDOW_CLASS_NAME, 
    L"DX Boiler", // TODO you should supply a window title
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,		
    0, 
    0, 
    640, // TODO you should supply width 
    480, // ..and height 
    NULL, 
    NULL, 
    g_hInst, 
    NULL);
 
  if (!hWnd)
  {
    return false; 
  }

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);
  g_hWnd = hWnd;

  if (InitDevice() != S_OK)
  {
    return false;
  }

  return true;
}

void DX11::Flip()
{
  // First check for events
  MSG  msg;
  while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  // The actual flip
  g_pSwapChain->Present(0, 0);
}

void DX11::Viewport(int x, int y, int w, int h)
{
  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)w;
  vp.Height = (FLOAT)h;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0; // TODO
  vp.TopLeftY = 0; // TODO
  g_pImmediateContext->RSSetViewports( 1, &vp );
}

void DX11::UseShader(DX11Shader* shader)
{
  assert(shader);
  m_currentShader = shader;
}

void DX11::InitFrame(float clearR, float clearG, float clearB)
{
  // clear the backbuffer
  float ClearColor[4] = { clearR, clearG, clearB, 1.0f }; //red,green,blue,alpha
  g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
  // And z buffer
  g_pImmediateContext->ClearDepthStencilView( g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DX11::SetPerspectiveProjection(
  float fov, float aspectRatio, float nearDist, float farDist)
{
  // Use RH version for compatibility with gluPerspective() etc
  g_Projection = XMMatrixPerspectiveFovLH(DegToRad(fov), aspectRatio, nearDist, farDist);
}

void DX11::LookAt(float eyeX, float eyeY, float eyeZ, float x, float y, float z, float upX, float upY, float upZ)
{
  XMVECTOR Eye = XMVectorSet(eyeX, eyeY, eyeZ, 0);
  XMVECTOR At = XMVectorSet(x, y, z, 0);
  XMVECTOR Up = XMVectorSet(upX, upY, upZ, 0);
  // Use RH Version
  g_View = XMMatrixLookAtLH(Eye, At, Up);
}

void DX11::Draw(DX11Drawable* vb)
{
  // Should be in shader
  //ConstantBuffer cb;
  ///cb.mWorld = XMMatrixTranspose( g_World );
  //cb.mView = XMMatrixTranspose( g_View ); // TODO Why transpose
  //cb.mProjection = XMMatrixTranspose( g_Projection );

  //  float view[16];
  //  DX11::GetMatrix(DX11_MODELVIEW_MATRIX, view);
  //  float proj[16];
  //  DX11::GetMatrix(DX11_PROJECTION_MATRIX, proj);

  //  cb.mView = XMMatrixTranspose(view); //DX11::GetMatrix(DX11_); // TODO Why transpose
  //  cb.mProjection = XMMatrixTranspose(proj);

  //g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );
  //g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );
  
  //// would like to: 
  m_currentShader->UseThisShader(g_pImmediateContext);

  vb->Draw(g_pImmediateContext, g_pd3dDevice, m_currentShader);
}

/*
void DX11::DrawTriList(const Tris& tris)
{
  assert(m_currentShader);

  int numTris = tris.size();

  // TODO Would be much better to create these once per mesh and reuse rather than
  //  continually recreating the buffers :-(
  // This is super inefficient but DX isn't the platform I really care about...
  ID3D11InputLayout*      g_pVertexLayout = NULL;
  ID3D11Buffer*           g_pVertexBuffer = NULL;

  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = D3D11_USAGE_DEFAULT;
  bd.ByteWidth = sizeof(Vert) * numTris * 3;
  bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  bd.CPUAccessFlags = 0;
  D3D11_SUBRESOURCE_DATA InitData;
  ZeroMemory(&InitData, sizeof(InitData));
  InitData.pSysMem = &tris[0]; //vertices;
  HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
  if (FAILED(hr))
    return;

  // Set vertex buffer
  UINT stride = sizeof(Vert);
  UINT offset = 0;
  g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

  // Set primitive topology
  g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Define the input layout
  D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
  };

  UINT numElements = ARRAYSIZE( layout );

  // Create the input layout
  hr = g_pd3dDevice->CreateInputLayout( layout, numElements, m_currentShader->GetBufferPointer(),
                                          m_currentShader->GetBufferSize(), &g_pVertexLayout );
  // TODO
  //pVSBlob->Release();

  if( FAILED( hr ) )
        return;

  // Set the input layout
  g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

  ConstantBuffer cb;
  ///cb.mWorld = XMMatrixTranspose( g_World );
  cb.mView = XMMatrixTranspose( g_View ); // TODO Why transpose
  cb.mProjection = XMMatrixTranspose( g_Projection );
  g_pImmediateContext->UpdateSubresource( g_pConstantBuffer, 0, NULL, &cb, 0, 0 );

  m_currentShader->UseThisShader(g_pImmediateContext);

  g_pImmediateContext->VSSetConstantBuffers( 0, 1, &g_pConstantBuffer );

  // Texturing
  g_pImmediateContext->PSSetShaderResources( 0, 1, &g_pTextureRV );
  g_pImmediateContext->PSSetSamplers( 0, 1, &g_pSamplerLinear );

  g_pImmediateContext->Draw(numTris * 3, 0);

  if( g_pVertexBuffer ) g_pVertexBuffer->Release();
  if( g_pVertexLayout ) g_pVertexLayout->Release();
}
*/

//void DX11::DrawIndexedTriList(
//  const Verts& verts,
//  const IndexedTriList& indexes)
//{
//  ;
//
//  // TODO
//}

void DX11::SetMatrixMode(MatrixMode m) 
{
  s_matrixMode = m;
}

void DX11::SetIdentity()
{
  if (s_matrixMode == DX11_MODELVIEW_MATRIX)
  {
    g_View = XMMatrixIdentity();
  }
  else if (s_matrixMode == DX11_PROJECTION_MATRIX)
  {
    g_Projection = XMMatrixIdentity();
  }
  else
  {
    assert(0);
  }
}

void DX11::MultMatrix(const float m[16])
{
  if (s_matrixMode == DX11_MODELVIEW_MATRIX)
  {
    g_View = XMMATRIX(m) * g_View;  // TODO Check this
  }
  else if (s_matrixMode == DX11_PROJECTION_MATRIX)
  {
    assert(0);
  }
  else
  {
    assert(0);
  }
}

void DX11::PushMatrix()
{
  assert(s_matrixMode != DX11_MATRIX_NOTSETYET);
  
  // Yechh, better to just always deal with the top of the current stack
  if (s_matrixMode == DX11_MODELVIEW_MATRIX)
  {
    mtxStack[(int)s_matrixMode].push(g_View);
  }
  else if (s_matrixMode == DX11_PROJECTION_MATRIX)
  {
    mtxStack[(int)s_matrixMode].push(g_Projection);
  }
  else
  {
    assert(0);
  }
}

void DX11::PopMatrix()
{
  assert(s_matrixMode != DX11_MATRIX_NOTSETYET);
  assert(!mtxStack[(int)s_matrixMode].empty());

  if (s_matrixMode == DX11_MODELVIEW_MATRIX)
  {
    g_View = mtxStack[(int)s_matrixMode].top();
    mtxStack[(int)s_matrixMode].pop();
  }
  else if (s_matrixMode == DX11_PROJECTION_MATRIX)
  {
    g_Projection = mtxStack[(int)s_matrixMode].top();
    mtxStack[(int)s_matrixMode].pop();
  }
  else
  {
    assert(0);
  }
}

void DX11::Translate(float x, float y, float z)
{
  assert(s_matrixMode == DX11_MODELVIEW_MATRIX);
  g_View = XMMatrixTranslation(x, y, z) * g_View;
}

void DX11::Scale(float x, float y, float z)
{
  assert(s_matrixMode == DX11_MODELVIEW_MATRIX);
  g_View = XMMatrixScaling(x, y, z) * g_View;
}

void DX11::RotateX(float degs)
{
  assert(s_matrixMode == DX11_MODELVIEW_MATRIX);
  g_View = XMMatrixRotationX(DegToRad(degs)) * g_View;
}

void DX11::RotateY(float degs)
{
  assert(s_matrixMode == DX11_MODELVIEW_MATRIX);
  g_View = XMMatrixRotationY(DegToRad(degs)) * g_View;
}

void DX11::RotateZ(float degs)
{
  if (s_matrixMode == DX11_MODELVIEW_MATRIX)
  {
    g_View = XMMatrixRotationZ(DegToRad(degs)) * g_View;
  }
  else
  {
    g_Projection = XMMatrixRotationZ(DegToRad(degs)) * g_Projection;
  }
}

void DX11::GetMatrix(MatrixMode mm, float result[16])
{
  switch (mm)
  {
  case DX11_MODELVIEW_MATRIX:
    memcpy(result, &g_View, 16 * sizeof(float)); 
    break;

  case DX11_PROJECTION_MATRIX:
    memcpy(result, &g_Projection, 16 * sizeof(float));     
    break;
  
  case DX11_TEXTURE_MATRIX:
    // Not supported
    break;
  }
}

/*
void DX11::Enable(unsigned int flag)
{
  switch (flag)
  {
  case AMJU_DEPTH_WRITE:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); //EnableZWrite(true);
    break;

  case AMJU_DEPTH_READ:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    break;

  case AMJU_LIGHTING:
    //dd->SetRenderState(D3DRS_LIGHTING, TRUE);
    //dd->LightEnable( 0, TRUE );
    //{
    //  const float A = 0.5f;
    //  dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(A, A, A, 1.0f ) );
    //}
    break;

  case AMJU_BLEND:
    //dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    break;

  case AMJU_TEXTURE_2D:
//    dd->SetTexture(0, mostRecentlyUsedTexture); // TODO
    break;
  }
}

void DX11::Disable(unsigned int flag)
{
  switch (flag)
  {
  case AMJU_DEPTH_WRITE:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AMJU_DEPTH_READ:
    //dd->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    break;

  case AMJU_LIGHTING:
    // TODO TEMP TEST
    // If lighting is disabled, set the ambient colour to white.
    // Then the material colour's ambient value will be used.
    //dd->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
//    dd->SetRenderState(D3DRS_LIGHTING, FALSE);
//    dd->LightEnable( 0, FALSE);
    break;

  case AMJU_BLEND:
    //dd->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    break;

  case AMJU_TEXTURE_2D:
    //dd->SetTexture(0, 0); 
    break;
  }
}
*/

void DX11::DestroyTextureHandle(TextureHandle*)
{
  // TODO Release texture 
}

static void CopyRGBATexture(unsigned char* texels, int pitch, const unsigned char* data, int width, int height)
{
  for (int y = 0; y < height; y++)
  {
    unsigned char* row = texels + y * pitch;

    for (int x = 0; x < width; x++)
    {
      row[0] = data[2]; 
      row[1] = data[1];
      row[2] = data[0];
      row[3] = data[3]; // Alpha

      row += 4;
      data += 4;
    }
  }
}

static void CopyRGBTexture(unsigned char* texels, int pitch, const unsigned char* data, int width, int height)
{
  for (int y = 0; y < height; y++)
  {
    unsigned char* row = texels + y * pitch;

    for (int x = 0; x < width; x++)
    {
        // RGB -> BGR
      row[0] = data[0];
      row[1] = data[1];
      row[2] = data[2];
      // RGB texture still has 16 bytes per pixel apprently.
      row[3] = 0;
      row += 4;
      data += 3;
    }
  }
}

void DX11::SetTexture(
  TextureHandle* th, 
  TextureType tt, 
  TextureDepth td,  
  int width, 
  int height, 
  unsigned char* data)
{
  D3D11_TEXTURE2D_DESC desc;
  ZeroMemory(&desc, sizeof(desc));
  desc.Width = width;
  desc.Height = height;
  desc.MipLevels = desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DYNAMIC;
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  ID3D11Texture2D *pTexture = NULL;

  // TODO If RGB, bulk data out to 32 bits
  unsigned char* padded = 0;
  if (td == AMJU_RGB)
  {
    padded = new unsigned char[4 * width * height];
    CopyRGBTexture(padded, width * 4, data, width, height);
  }

  D3D11_SUBRESOURCE_DATA subr;
  subr.pSysMem = padded ? padded : data;
  subr.SysMemPitch = width * 4;
  subr.SysMemSlicePitch = 0;
  HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, &subr, &pTexture);

  // Ok to del padded data here ?
  delete [] padded;

  D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
  D3D11_RESOURCE_DIMENSION type;
  pTexture->GetType(&type);
  assert(type == D3D11_RESOURCE_DIMENSION_TEXTURE2D);
		
  srvDesc.Format = desc.Format;
  srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDesc.Texture2D.MipLevels = desc.MipLevels;
  srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;

  hr = g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &g_pTextureRV);

  *th = reinterpret_cast<int>(g_pTextureRV); // echh...
}

void DX11::UseTexture(TextureHandle th)
{
  g_pTextureRV = reinterpret_cast<ID3D11ShaderResourceView*>(th);
}

