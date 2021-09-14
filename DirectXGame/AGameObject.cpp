#include "AGameObject.h"

#include "History.h"

AGameObject::AGameObject(std::string name)
{
    this->name = name;
    this->localPosition = Vector3D();
    this->localRotation = Vector3D();
    this->localScale = Vector3D(1, 1, 1);
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

void AGameObject::saveState()
{
    this->storedPosition = Vector3D(this->localPosition);
    this->storedRotation = Vector3D(this->localRotation);
    this->storedScale = Vector3D(this->localScale);
}

void AGameObject::restoreState()
{
    setPosition(storedPosition);
    setRotation(storedRotation);
    setScale(storedScale);

    for (int i = 0; i < componentList.size(); i++)
    {
        componentList[i]->reset();
    }
}

void AGameObject::awake()
{

}
