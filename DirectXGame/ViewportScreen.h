#pragma once
#include "AUIScreen.h"
#include <d3d11.h>

class ViewportScreen : public AUIScreen
{
public:
	ViewportScreen();
	~ViewportScreen();

	virtual void drawUI() override;

	void setTex(ID3D11ShaderResourceView* tex);

private:
	ID3D11ShaderResourceView* texToShow;

	friend class UIManager;
};