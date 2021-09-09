#include "InspectorScreen.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "GameObjectManager.h"
#include "AGameObject.h"

InspectorScreen::InspectorScreen() : AUIScreen("Inspector")
{
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::drawUI()
{
	ImGui::Begin("Inspector Window");
	ImGui::SetWindowPos(ImVec2(UIManager::WINDOW_WIDTH - 200, 20));
	ImGui::SetWindowSize(ImVec2(250, UIManager::WINDOW_HEIGHT));
	this->selectedObject = GameObjectManager::getInstance()->getSelectedObject();
	if (this->selectedObject != NULL) {
		std::string name = this->selectedObject->getName();
		ImGui::Text("Selected Object: %s", name.c_str());

		this->updateTransformDisplays();
		bool enabled = this->selectedObject->isEnabled();
		if (ImGui::InputFloat3("Position", this->Displaiedposition)) { this->onTransformUpdate(); }
		if (ImGui::InputFloat3("Rotation", this->Displaiedrotation)) { this->onTransformUpdate(); }
		if (ImGui::InputFloat3("Scale", this->Displaiedscale)) { this->onTransformUpdate(); }

	}
	ImGui::End();
}

void InspectorScreen::updatePosition()
{
	Vector3D pos = this->selectedObject->getLocalPosition();
	this->Displaiedposition[0] = pos.x;
	this->Displaiedposition[1] = pos.y;
	this->Displaiedposition[2] = pos.z;

	Vector3D rot = this->selectedObject->getLocalRotation();
	this->Displaiedrotation[0] = rot.x;
	this->Displaiedrotation[1] = rot.y;
	this->Displaiedrotation[2] = rot.z;

	Vector3D scale = this->selectedObject->getLocalScale();
	this->Displaiedscale[0] = scale.x;
	this->Displaiedscale[1] = scale.y;
	this->Displaiedscale[2] = scale.z;
}

void InspectorScreen::updateScale()
{
	if (this->selectedObject != nullptr) {
		this->selectedObject->setPosition(Vector3D(this->Displaiedposition[0], this->Displaiedposition[1], this->Displaiedposition[2]));
		this->selectedObject->setRotation(Vector3D(this->Displaiedrotation[0], this->Displaiedrotation[1], this->Displaiedrotation[2]));
		this->selectedObject->setScale(Vector3D(this->Displaiedscale[0], this->Displaiedscale[1], this->Displaiedscale[2]));
	}
}
