#pragma once
#include <string>
#include "GameObjectConstants.h"
#include "AComponent.h"
#include <vector>

class VertexShader;
class PixelShader;
class EditorAction;
class AGameObject
{
public:
	enum PrimitiveType {
		CAMERA,
		CUBE,
		PLANE,
		SPHERE,
		CAPSULE,
		CYLINDER
	};


	AGameObject(std::string name, PrimitiveType type);
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

	PrimitiveType getObjectType();

	std::string getName();
	void setName(std::string newName);

	void attachComponent(AComponent* component);
	void detachComponent(AComponent* component);

	AComponent* findComponentByName(std::string name);
	AComponent* findComponentOfType(AComponent::ComponentType type, std::string name);
	std::vector<AComponent*> getComponentsOfType(AComponent::ComponentType type);
	std::vector<AComponent*> getAllComponents();

	void setParent(AGameObject* parentObject);
	void addChild(AGameObject* childObject);
	void removeChild(AGameObject* childObject);
	void removeParent();
	AGameObject* getParent();
	std::vector<AGameObject*> getAllChildren();

	void saveState();
	virtual void restoreState();

	void saveEditState();
	void restoreEditState();

protected:
	virtual void updateVertexLocations();

	std::string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	PrimitiveType objectType;

	std::vector<AComponent*> componentList;

	EditorAction* lastEditState = nullptr;

	//Parenting
	AGameObject* parent = nullptr;
	std::vector<AGameObject*> childList;
};

