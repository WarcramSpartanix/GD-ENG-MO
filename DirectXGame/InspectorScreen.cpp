#include "InspectorScreen.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "ActionHistory.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"

InspectorScreen::InspectorScreen() : AUIScreen("Inspector")
{
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::drawUI()
{
	ImGui::Begin("Inspector Window");
	selectedObject = GameObjectManager::getInstance()->getSelectedObject();



	if (selectedObject != nullptr) {
		std::string name = selectedObject->getName();
		ImGui::Text("Selected Object: %s", name.c_str());

		Vector3D position = selectedObject->getLocalPosition();
		Vector3D rotation = selectedObject->getLocalRotation();
		Vector3D objectScale = selectedObject->getLocalScale();

		pos[0] = position.x;
		pos[1] = position.y;
		pos[2] = position.z;
		
		rot[0] = rotation.x;
		rot[1] = rotation.y;
		rot[2] = rotation.z;
		
		scale[0] = objectScale.x;
		scale[1] = objectScale.y;
		scale[2] = objectScale.z;

		if (ImGui::InputFloat3("Position", pos)) { updatePosition(); }
		if (ImGui::InputFloat3("Rotation", rot)) { updateRotation(); }
		if (ImGui::InputFloat3("Scale", scale)) { updateScale(); }

		std::vector<AComponent*> componentList = selectedObject->getComponentsOfType(AComponent::ComponentType::Physics);
		ImGui::Text("PhysicsComponent"); ImGui::SameLine();
		if (componentList.size() > 0)
		{
			if (ImGui::Button("RemovePhysics"))
			{
				selectedObject->detachComponent(componentList[0]);
			}
		}
		else
		{
			if (ImGui::Button("AddPhysics")) selectedObject->attachComponent(new PhysicsComponent("PhysicsComponent", selectedObject));
		}

		componentList = selectedObject->getComponentsOfType(AComponent::ComponentType::Renderer);
		ImGui::Text("TextureComponent"); ImGui::SameLine();
		if (componentList.size() > 0)
		{
			if (ImGui::Button("RemoveTexture"))
			{
				selectedObject->detachComponent(componentList[0]);
			}
		}
		else
		{
			if (ImGui::Button("AddTexture")) selectedObject->attachComponent(new TextureComponent("TextureComponent", selectedObject));
		}


		//Set parent
		updateParent();
		updateChildren();
		//

		if (ImGui::Button("Delete Object", ImVec2(235, 0)))	{ GameObjectManager::getInstance()->deleteObject(selectedObject->getName().c_str());}

	}
	ImGui::End();
}

void InspectorScreen::updatePosition()
{
	if (selectedObject != nullptr)
	{
		ActionHistory::getInstance()->recordAction(this->selectedObject, false);
		selectedObject->setPosition(Vector3D(pos[0], pos[1], pos[2]));
	}
}

void InspectorScreen::updateChildrenPosition(std::vector<AGameObject*> children, Vector3D changeinPosition)
{
}




void InspectorScreen::updateRotation()
{
	if (selectedObject != nullptr)
	{
		ActionHistory::getInstance()->recordAction(this->selectedObject, false);
		selectedObject->setRotation(Vector3D(rot[0], rot[1], rot[2]));
	}
}

void InspectorScreen::updateScale()
{
	if (selectedObject != nullptr)
	{
		ActionHistory::getInstance()->recordAction(this->selectedObject, false);
		selectedObject->setScale(Vector3D(scale[0], scale[1], scale[2]));
	}
}

void InspectorScreen::updateParent()
{
	if (selectedObject->getParent() == nullptr)
		s = "None";
	else
		s = selectedObject->getParent()->getName();

	ImGui::Text("Current Parent: %s", s.c_str());
	if (selectedObject->getParent() != nullptr)
	{
		if (ImGui::Button("Remove Parent"))
		{
			AGameObject* temp = selectedObject->getParent();
			temp->removeChild(selectedObject);
			selectedObject->removeParent();
		}
	}

	ImGui::InputTextWithHint("", "Parent object's name", parentName, sizeof(parentName)); ImGui::SameLine();

	if (ImGui::Button("Set"))
	{
		s.clear(); s = parentName;
		AGameObject* temp = GameObjectManager::getInstance()->findObjectByName(s);
		

		//if parent is not null or not the selected objec
		if (temp != nullptr && temp->getName() != selectedObject->getName() )
		{
			bool isFound = false;
			bool hasChildren = false;

			std::vector<AGameObject*> tempChildren = selectedObject->getAllChildren();
			if (tempChildren.size() > 0)//check if parent is child of object
			{
				hasChildren = true;
				for (int i = 0; i < tempChildren.size(); i++)
				{
					if (tempChildren[i]->getName() == temp->getName())
						isFound = true;
				}
			}
			
			// if no children in the first place or has children but parent is not one of its children
			if (!isFound || !hasChildren)
			{
				bool canAddChild = false;
				if (selectedObject->getParent() == nullptr)
					canAddChild = true;
				else if(selectedObject->getParent()->getName() != temp->getName())
					canAddChild = true;

				if (canAddChild)// just a checker if assigned parent isnt current parent
				{
					selectedObject->setParent(temp);
					temp->addChild(selectedObject);
				}	
			}
		}	
	}
}

void InspectorScreen::updateChildren()
{
	std::vector<AGameObject*> tempChildren = selectedObject->getAllChildren();
	ImGui::Text("Current Children: %d", tempChildren.size());
	if (tempChildren.size() > 0)
	{
		for (int i = 0; i < tempChildren.size(); i++)
		{
			ImGui::Text("i = %d, %s", i , tempChildren[i]->getName().c_str());
		}
	}
}
