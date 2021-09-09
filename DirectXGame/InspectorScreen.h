#pragma once
#include "AUIScreen.h"
class InspectorScreen : public AUIScreen
{
public:
	InspectorScreen();
	~InspectorScreen();

	// Inherited via AUIScreen
	virtual void drawUI() override;

	float Displaiedposition[3] = { 0.0f, 0.0f, 0.0f };
	float Displaiedrotation[3] = { 0.0f, 0.0f, 0.0f };
	float Displaiedscale[3] = { 1.0f, 1.0f, 1.0f };

	class AGameObject* selectedObject;

	float pos[3];
	float rot[3];
	float scale[3];
};

