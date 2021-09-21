#pragma once
#include "AGameObject.h"
class Sphere : public AGameObject
{
public:
	Sphere(std::string name, Vector3D pos, float radius, int tessellationLevel);
	~Sphere();

	virtual void update(float deltaTime) override;
	virtual void draw(ConstantBuffer* cb) override;

	virtual void setPosition(float x, float y, float z) override;
	virtual void setPosition(Vector3D pos) override;



	void setRadius(float newRadius);
	void setTessellationLevel(int newTessellationLevel);

	float getRadius();

	std::vector<Vector3D> getVertexWorldPositions();

	float checkRaycast(Vector3D rayOrigin, Vector3D rayDirection);

private:
	virtual void updateVertexLocations() override;

	void generateEdgesAtTessellation(int tessellation);

	std::vector<Vector3D> edges;
	std::vector<Vector2D> texCoords;
	float radius;
	int tessellationLevel;

	class VertexBuffer* m_vb;
	class VertexShader* m_vs;
	class PixelShader* m_ps;
	class IndexBuffer* m_ib;

	class BoundingSphere* collisionSphere;

	class Texture* m_default_tex;

	class Mesh* m_mesh;
};

