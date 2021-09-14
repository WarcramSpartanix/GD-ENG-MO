#pragma once
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "AGameObject.h"


class Storage
{
public:
	Storage(AGameObject* gameObject);
	~Storage();

	std::string getOwnerName();
	Vector3D getStoredPos();
	Vector3D gettStoredScale();
	Vector3D getStoredRotation();
	Matrix4x4 getStoredMatrix();

private:
	std::string name;
	Vector3D pos;
	Vector3D scale;
	Vector3D rot;
	Matrix4x4 mat;
};