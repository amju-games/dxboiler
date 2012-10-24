#include "FullScreenQuad.h"
#include "FullScreenShader.h"

void FullScreenQuad::Draw( ID3D11DeviceContext*context, ID3D11Device*, DX11Shader* shader )
{
	UINT strides = sizeof(float)*5;
	UINT offsets = 0;
	context->IASetVertexBuffers(0,1,&m_pVertexBuffer,&strides,&offsets);
	context->IASetInputLayout(m_pVertexLayout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->Draw(6,0);
}

bool FullScreenQuad::Load( ID3D11Device* dx11dev,DX11Shader* shader)
{
	float verts[30] = {-1,1,0,
					      0,0,
	                   1,-1,0,
					      1,1,
				      -1,-1,0,
						  0,1,
					   -1,1,0,
						  0,0,
						1,1,0,
						  1,0,
					   1,-1,0,
					      1,1};
	
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.CPUAccessFlags = 0;
	desc.ByteWidth = sizeof(float)*30;
	desc.StructureByteStride = 0;
	desc.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = verts;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT result = dx11dev->CreateBuffer(&desc,&data,&m_pVertexBuffer);

	if (result != S_OK)
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = dx11dev->CreateInputLayout(layout,2,shader->GetBufferPointer(),shader->GetBufferSize(),&m_pVertexLayout);

	if (result != S_OK)
	{
		return false;
	}

	return true;
}
