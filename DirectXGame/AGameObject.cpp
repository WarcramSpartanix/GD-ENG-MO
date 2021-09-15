#include "AGameObject.h"

#include <iostream>

#include "EditorAction.h"

AGameObject::AGameObject(std::string name, AGameObject::PrimitiveType type)
{
    this->name = name;
    this->localPosition = Vector3D();
    this->localRotation = Vector3D();
    this->localScale = Vector3D(1, 1, 1);
    this->objectType = type;
}

AGameObject::~AGameObject()
{
}

void AGameObject::setPosition(float x, float y, float z)
{
    this->localPosition = Vector3D(x, y, z);
    updateVertexLocations();
}

void AGameObject::setPosition(Vector3D pos)
{
    this->localPosition = pos;
    updateVertexLocations();
}

Vector3D AGameObject::getLocalPosition()
{
    return this->localPosition;
}

void AGameObject::setScale(float x, float y, float z)
{
    this->localScale = Vector3D(x, y, z);
    updateVertexLocations();
}

void AGameObject::setScale(Vector3D scale)
{
    this->localScale = scale;
    updateVertexLocations();
}

Vector3D AGameObject::getLocalScale()
{
    return this->localScale;
}

void AGameObject::setRotation(float x, float y, float z)
{
    this->localRotation = Vector3D(x, y, z);
    updateVertexLocations();
}

void AGameObject::setRotation(Vector3D rot)
{
    this->localRotation = rot;
    updateVertexLocations();
}

Vector3D AGameObject::getLocalRotation()
{
    return this->localRotation;
}

AGameObject::PrimitiveType AGameObject::getObjectType()
{
    return this->objectType;
}

std::string AGameObject::getName()
{
    return this->name;
}

void AGameObject::setName(std::string newName)
{
    name = newName;
}

void AGameObject::attachComponent(AComponent* component)
{
    this->componentList.push_back(component);
}

void AGameObject::detachComponent(AComponent* component)
{
    for (int i = 0; i < componentList.size(); i++)
    {
        if (componentList[i] == component)
        {
            componentList.erase(componentList.begin() + i);
        }
    }
    delete component;
}

AComponent* AGameObject::findComponentByName(std::string name)
{
    for (int i = 0; i < this->componentList.size(); i++)
    {
        if (this->componentList[i]->getName() == name)
            return this->componentList[i];
    }
    return nullptr;
}

AComponent* AGameObject::findComponentOfType(AComponent::ComponentType type, std::string name)
{
    for (int i = 0; i < this->componentList.size(); i++)
    {
        if (this->componentList[i]->getName() == name && this->componentList[i]->getType() == type)
            return this->componentList[i];
    }
    return nullptr;
}

std::vector<AComponent*> AGameObject::getComponentsOfType(AComponent::ComponentType type)
{
    std::vector<AComponent*> out;
    for (int i = 0; i < this->componentList.size(); i++)
    {
        if (this->componentList[i]->getType() == type)
            out.push_back(this->componentList[i]);
    }
    return out;
}

std::vector<AComponent*> AGameObject::getAllComponents()
{
    return componentList;
}

void AGameObject::setParent(AGameObject* parentObject)
{
    this->parent = parentObject;
}

void AGameObject::addChild(AGameObject* childObject)
{
    this->childList.push_back(childObject);
}

void AGameObject::removeChild(AGameObject* childObject)
{
    int index = -1;
    for (int i = 0; i < this->childList.size(); i++) {
        if (this->childList[i] == childObject) {
            index = i;
        }
        
    }

    if (index != -1) {
        //std::cout << "Child '" << this->childList[index]->getName() << "' has been removed from Parent '" << this->parent->getName() << "'\n";
        this->childList.erase(this->childList.begin() + index);
        
    }
}

void AGameObject::removeParent()
{
    this->parent = nullptr; 
}

AGameObject* AGameObject::getParent()
{
    if (this->parent != nullptr)
        return this->parent; 
        
    return nullptr;
}

std::vector<AGameObject*> AGameObject::getAllChildren()
{
    return this->childList;
}

void AGameObject::saveState()
{
}

void AGameObject::restoreState()
{
}

void AGameObject::saveEditState()
{
    if (this->lastEditState == nullptr) {
        this->lastEditState = new EditorAction(this);
    }
}

void AGameObject::restoreEditState()
{
    if (this->lastEditState != nullptr) {
        this->localPosition = this->lastEditState->getStorePos();
        std::cout << localPosition.y;
        this->localScale = this->lastEditState->getStoredScale();
        this->localRotation = this->lastEditState->getStoredOrientation();

        this->lastEditState = nullptr;

        for (int i = 0; i < componentList.size(); i++)
        {
            componentList[i]->reset();
        }
        updateVertexLocations();
    }
}
