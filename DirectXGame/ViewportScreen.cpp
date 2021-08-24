#include "ViewportScreen.h"
#include "imGUI/imgui.h"
#include "imGUI/imgui_impl_dx11.h"
#include "imGUI/imgui_impl_win32.h"
#include "UIManager.h"

ViewportScreen::ViewportScreen() : AUIScreen("Viewport")
{
}

ViewportScreen::~ViewportScreen()
{
}

void ViewportScreen::drawUI()
{
	ImGui::Begin("Camera Viewport");
	ImVec2 wsize = ImGui::GetWindowSize();
	ImGui::Image(texToShow, wsize);
	ImGui::End();
}

void ViewportScreen::setTex(ID3D11ShaderResourceView* tex)
{
	texToShow = tex;
}
