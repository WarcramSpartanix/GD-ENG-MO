#pragma once
#include "GameObjectConstants.h"
#include "AGameObject.h"

class EditorAction
{
public:
	EditorAction(AGameObject* gameObject);
	~EditorAction();

	std::string getOwnerName();
	Vector3D getStorePos();
	Vector3D getStoredScale();
	Vector3D getStoredOrientation();

private:
	std::string objectName;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D orientation;
};

