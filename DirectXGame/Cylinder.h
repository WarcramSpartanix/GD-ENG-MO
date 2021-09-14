#pragma once
#include "AGameObject.h"
#include <vector>

class Cylinder : public AGameObject
{
public:
	Cylinder(std::string name, float height, float radius, int sectorCount);
	~Cylinder();

	void setColors(Vector3D color);
	virtual void update(float deltaTime) override;
	virtual void draw(ConstantBuffer* cb) override;

	virtual void setPosition(float x, float y, float z) override;
	virtual void setPosition(Vector3D pos) override;

	virtual void setScale(float x, float y, float z) override;
	virtual void setScale(Vector3D scale) override;

	virtual void setRotation(float x, float y, float z) override;
	virtual void setRotation(Vector3D rot) override;

	void restoreState() override;
	void attachPhysicsComponent();
	std::vector<Vector3D> getVertexWorldPositions();

	float checkRaycast(Vector3D rayOrigin, Vector3D rayDirection);

private:
	void buildVerticesAndIndices();
	std::vector<float> getUnitCircleVertices();
	std::vector<float> m_vertices;
	std::vector<Vector2D> m_texCoords;
	std::vector<float> m_indices;
	

	int m_sectorCount;
	float m_height;
	float m_radius;

private:
	virtual void updateVertexLocations() override;
	void updateVertexLocations(std::vector<Vector3D> edges);

	Vector3D colors;
	Vector3D colors2[8];

	float rot_x = 0;
	float rot_y = 0;
	float speed = 1;

	std::vector<Vector3D> edges;

	class Texture* m_default_tex;

	class VertexBuffer* m_vb;
	class VertexShader* m_vs;
	class PixelShader* m_ps;
	class IndexBuffer* m_ib;

	class BoundingBox* collisionBox;
};

