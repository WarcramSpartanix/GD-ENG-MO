#pragma once
#include "Camera.h"
class GameCamera :
    public Camera
{
public:
	GameCamera(std::string name, Vector3D pos);
	~GameCamera();

	void initializeMesh();

	virtual void draw(ConstantBuffer* cb) override;

	Vector3D* getVertexWorldPositions();

private:
	virtual void updateVertexLocations() override;

	Vector3D edges[8];

	class VertexBuffer* m_vb;
	class VertexShader* m_vs;
	class PixelShader* m_ps;
	class IndexBuffer* m_ib;

	class BoundingBox* collisionBox;
};

