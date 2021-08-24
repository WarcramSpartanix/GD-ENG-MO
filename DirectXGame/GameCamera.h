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

	virtual void setPosition(float x, float y, float z) override;
	virtual void setPosition(Vector3D pos) override;

	virtual void setScale(float x, float y, float z) override;
	virtual void setScale(Vector3D scale) override;

	virtual void setRotation(float x, float y, float z) override;
	virtual void setRotation(Vector3D rot) override;

	Vector3D* getVertexWorldPositions();

	float checkRaycast(Vector3D rayOrigin, Vector3D rayDirection);

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& delta_mouse_pos) override;

private:
	virtual void updateVertexLocations() override;

	Vector3D edges[13];

	class VertexBuffer* m_vb;
	class VertexShader* m_vs;
	class PixelShader* m_ps;
	class IndexBuffer* m_ib;

	class BoundingBox* collisionBox;
};

