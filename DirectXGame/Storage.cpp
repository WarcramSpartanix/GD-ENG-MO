#pragma once
#include "Storage.h"

Storage::Storage(AGameObject* gameObject)
{
	this->name = gameObject->getName();
	this->pos = gameObject->getLocalPosition();
	this->scale = gameObject->getLocalScale();
	this->rot = gameObject->getLocalRotation();

}

Storage::~Storage()
{
}

std::string Storage::getOwnerName()
{
	return this->name;
}

Vector3D Storage::getStoredPos()
{
	return this->pos;
}

Vector3D Storage::gettStoredScale()
{
	return this->scale;
}

Vector3D Storage::getStoredRotation()
{
	return this->rot;
}

Matrix4x4 Storage::getStoredMatrix()
{
	return this->mat;
}
