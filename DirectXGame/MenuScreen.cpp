#include "MenuScreen.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <iostream>
#include "GraphicsEngine.h"
#include "VertexShader.h"
#include "UIManager.h"
#include "GameCamera.h"
#include "GameObjectManager.h"
#include "CameraManager.h"
#include "Cube.h"


MenuScreen::MenuScreen() : AUIScreen("Menu")
{
}

MenuScreen::~MenuScreen()
{
}

void MenuScreen::drawUI()
{
	if(ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Exit Editor", "Ctrl+W")) {/*Do something */ }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Game Object")) {
			if (ImGui::MenuItem("Create Camera")) { onCreateCameraClick(); }
			if (ImGui::MenuItem("Align With View")) { onClickAlignWithView(); }
			if (ImGui::BeginMenu("Light")) {
				if (ImGui::MenuItem("Point Light")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if(ImGui::BeginMenu("Start Game"))
		{
			if (ImGui::MenuItem("Play")) { onClickPlay(); }
			if (ImGui::MenuItem("Stop")) { onClickStop(); }

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void MenuScreen::onCreateCameraClick()
{
	if (CameraManager::getInstance()->getGameCam() == nullptr)
	{
		GameObjectManager::getInstance()->addObject(new GameCamera("GameCamera", Vector3D(0, 0, 0)));
		UIManager::getInstance()->addViewport();
	}
}

void MenuScreen::onClickPlay()
{
	CameraManager::getInstance()->setActiveCamera(CameraManager::CameraType::GAME_CAMERA);
}

void MenuScreen::onClickStop()
{
	CameraManager::getInstance()->setActiveCamera(CameraManager::CameraType::SCENE_CAMERA);
}

void MenuScreen::onClickAlignWithView()
{
	if (CameraManager::getInstance()->getGameCam() != nullptr)
	{
		CameraManager::getInstance()->alignView();
	}
}
