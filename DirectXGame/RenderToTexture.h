#pragma once
#include <d3d11.h>

class RenderToTexture
{
public:
	RenderToTexture();
	~RenderToTexture();

	bool Initialize(int width, int height);
	void Destroy();

	ID3D11RenderTargetView** getRenderTargetView();
	ID3D11ShaderResourceView* getShaderResourceView();

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};

