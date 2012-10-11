#ifndef FullScreenQuad_h__
#define FullScreenQuad_h__

#include "DX11Drawable.h"


class FullScreenQuad : public DX11Drawable
{
public:
	void Draw(ID3D11DeviceContext*, ID3D11Device*, DX11Shader* shader);
	bool Load(ID3D11Device*,DX11Shader* shader);
};


#endif // FullScreenQuad_h__
