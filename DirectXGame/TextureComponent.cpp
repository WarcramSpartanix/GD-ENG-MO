#include "TextureComponent.h"
#include "GraphicsEngine.h"
#include <codecvt>

TextureComponent::TextureComponent(std::string name, AGameObject* owner) : AComponent(name, ComponentType::Renderer, owner)
{
	filepath = L"Assets\\Textures\\wood.jpg";
	texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(filepath.c_str());
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
	filepath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(path);
	texture = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(filepath.c_str());
}

void TextureComponent::reset()
{
}

std::string TextureComponent::getPath()
{
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(filepath);
}


