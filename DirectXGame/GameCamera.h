#pragma once
#include "Camera.h"
class GameCamera :
    public Camera
{
	GameCamera(std::string name);
	~GameCamera();

	virtual void draw(ConstantBuffer* cb) override;
};

