#pragma once
#include <string>
#include "GameObjectConstants.h"
#include "AComponent.h"
#include <vector>

#include "PhysicsComponent.h"

class VertexShader;
class PixelShader;
class GameObjectManager;
class AGameObject
{
public:
	AGameObject(std::string name);
	~AGameObject();

	virtual void update(float deltaTime) = 0;
	virtual void draw(class ConstantBuffer* cb) = 0;

	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	virtual void setScale(float x, float y, float z);
	virtual void setScale(Vector3D scale);
	Vector3D getLocalScale();

	virtual void setRotation(float x, float y, float z);
	virtual void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

	std::string getName();
	void setName(std::string newName);

	void attachComponent(AComponent* component);
	void detachComponent(AComponent* component);

	AComponent* findComponentByName(std::string name);
	AComponent* findComponentOfType(AComponent::ComponentType type, std::string name);
	std::vector<AComponent*> getComponentsOfType(AComponent::ComponentType type);
	std::vector<AComponent*> getAllComponents();

	void saveState();
	virtual void restoreState();

protected:
	virtual void updateVertexLocations() = 0;

	std::string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	Vector3D storedPosition;
	Vector3D storedScale;
	Vector3D storedRotation;
	Matrix4x4 storedMatrix;
	bool stored = false;
	std::vector<AComponent*> componentList;
	PhysicsComponent* component;
	virtual void awake();

private:
	friend class GameObjectManager;
};

