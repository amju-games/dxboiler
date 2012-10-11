#ifndef FullScreenShader_h__
#define FullScreenShader_h__

#include <DX11Shader.h>
#include <InputHandlers.h>

struct cbFullScreen
{
	XMFLOAT2 mouseCoord;
	XMFLOAT2 padding;
};

class FullScreenShader : public DX11ShaderWithCB<cbFullScreen>,public MouseInputHandler
{
public:
	void UpdateConstantBuffer();
	bool ProcessMouseEvent(MouseState&);
private:
	XMFLOAT2 m_mouseCoord;
};


#endif // FullScreenShader_h__
