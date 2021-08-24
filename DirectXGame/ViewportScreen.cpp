#include "ViewportScreen.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_dx11.h"
#include "imGUI/imgui_impl_win32.h"
#include "UIManager.h"

ViewportScreen::ViewportScreen(int num) : AUIScreen("Viewport"), index(num)
{
}

ViewportScreen::~ViewportScreen()
{
}

void ViewportScreen::drawUI()
{
	std::string name = "Camera Viewport " + std::to_string(index);
	ImGui::Begin(name.c_str());

	ImVec2 windowSize = ImGui::GetWindowSize();
	ImGui::Image(texToShow, windowSize);

	ImGui::End();
}

void ViewportScreen::setTex(ID3D11ShaderResourceView* tex)
{
	texToShow = tex;
}
