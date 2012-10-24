/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.

FULL SCREEN TEXTURE WITH POSTPROCESSING EFFECT
*/

#include <iostream>
#include <DX11.h>
#include <ReportError.h>
#include <DX11VertexBuffer.h>
#include <DX11Texture.h>
#include "FullScreenShader.h"
#include "FullScreenQuad.h"

int main(int argc, char **argv)
{
	std::cout << "Using DX11, woohoo\n";

	if (!DX11::CreateWindow()) 
	{
		ReportError("Failed to create window!?");
		return 1;
	}

	DX11Texture texture;

	texture.Load("cake.jpg");

	DX11Shader* shader = new FullScreenShader; 
	// You derive from DX11Shader to do app-specific stuff 
	// Subclasses have vars corresponding to uniform vars in shader

	if (!shader->Load("FullScreenShader.fx"))
	{
		return 1;
	}

	DX11::GetInput()->AddMouseHandler(1,dynamic_cast<MouseInputHandler*>(shader));
	DX11::UseShader(shader);
	DX11::Viewport(0, 0, 640, 480);

	DX11::SetMatrixMode(DX11_PROJECTION_MATRIX);
	DX11::SetIdentity();
	DX11::SetPerspectiveProjection(45.0f, 1.3f, 1.0f, 1000.0f);

	FullScreenQuad fQuad;
	fQuad.Load(DX11::GetDevice(),shader);

	while (true)
	{
		DX11::GetInput()->Update();
		DX11::InitFrame(0, 0, 1); // clear colour

		DX11::SetMatrixMode(DX11_MODELVIEW_MATRIX);
		DX11::SetIdentity();
		DX11::LookAt(0, 0, 10,  0, 0, 0,  0, 1, 0);

		shader->UseThisShader(DX11::GetImmediateContext());
		texture.UseThisTexture(DX11::GetImmediateContext());
		fQuad.Draw(DX11::GetImmediateContext(),DX11::GetDevice(),shader);
		DX11::Flip();
	}

	return 0;
}
