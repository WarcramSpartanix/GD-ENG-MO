#pragma once
#include "AComponent.h"
#include <reactphysics3d/reactphysics3d.h>

class PhysicsComponent : public AComponent
{
public:
	PhysicsComponent(std::string name, AGameObject* owner, bool isSphere = false);
	~PhysicsComponent();

	void perform(float deltaTime) override;
	reactphysics3d::RigidBody* getRigidBody();
	void reset() override;

private:
	float mass = 1.0f;
	reactphysics3d::RigidBody* rigidBody;
	bool m_isSphere = false;
};

