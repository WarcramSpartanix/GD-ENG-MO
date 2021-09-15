#include "TextureComponent.h"
#include "GraphicsEngine.h"

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, ComponentType::Renderer, owner)
{
	std::wstring path = L"Assets\\Textures\\wood.jpg";
	filepath = "Assets/Textures/wood.jpg";
	texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(path.c_str());
}

TextureComponent::~TextureComponent()
{
}

void TextureComponent::perform(float deltaTime)
{
}

Texture* TextureComponent::getTexture()
{
	return texture;
}

void TextureComponent::setPathAndLoad(std::string path)
{
	filepath = path;
	std::wstring str = std::wstring(path.begin(), path.end());
	texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(str.c_str());
}

void TextureComponent::reset()
{
}

std::string TextureComponent::getPath()
{
	return filepath;
}


