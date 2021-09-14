#pragma once
#include "AComponent.h"


class Texture;
class TextureComponent :
    public AComponent
{
public:
	TextureComponent(std::string name, AGameObject* owner);
	~TextureComponent();

	void perform(float deltaTime) override;
	Texture* getTexture();
	void setPathAndLoad(std::string path);
	void reset() override;
	std::string getPath();

private:
	std::wstring filepath;
	Texture* texture;
};

