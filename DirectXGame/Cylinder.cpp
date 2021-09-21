#include "Cylinder.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"
#include "BoundingBox.h"
#include <random>
#include "PhysicsComponent.h"
#include "TextureComponent.h"

Cylinder::Cylinder(std::string name, float height, float radius, int sectorCount) : AGameObject(name, AGameObject::PrimitiveType::CYLINDER)
{
	colors = Vector3D(0, 1, 1);
	m_height = height;
	m_radius = radius;
	m_sectorCount = sectorCount;

	buildVerticesAndIndices();

	this->attachComponent(new PhysicsComponent("CylinderPhysics", this, false));
	m_default_tex = GraphicsEngine::getInstance()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\White.png");

	for (int i = 0; i < m_vertices.size() / 3; i++)
	{
		edges.push_back(Vector3D(m_vertices[i * 3], m_vertices[i * 3 + 1], m_vertices[i * 3 + 2]));
	}

	collisionBox = new BoundingBox(this->localPosition, this->localRotation, m_radius, m_height, m_radius);

	RenderSystem* graphEngine = GraphicsEngine::getInstance()->getRenderSystem();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	std::vector<Vector3D> worldLocations = getVertexWorldPositions();
	vertex* vertexList = (vertex*)malloc(sizeof(vertex) * edges.size());

	

	for (int i = 0; i < edges.size(); i++)
	{
		vertexList[i] = { worldLocations[i], m_texCoords[i] };
	}

	UINT size_list = edges.size();
	m_vb = graphEngine->createVertexBuffer(vertexList, sizeof(vertex), size_list, shader_byte_code, size_shader);

	

	unsigned int* index_list = (unsigned int*)malloc(sizeof(unsigned int) * (m_indices.size()));

	for (int i = 0; i < m_indices.size(); i++)
	{
		index_list[i] = m_indices[i];
	}

	
	UINT size_index_list = m_indices.size();
	m_ib = graphEngine->createIndexBuffer(index_list, size_index_list);

	m_vs = graphEngine->createVertexShader(shader_byte_code, size_shader);


	graphEngine->releaseCompiledShader();

	graphEngine->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = graphEngine->createPixelShader(shader_byte_code, size_shader);

	graphEngine->releaseCompiledShader();
}

Cylinder::~Cylinder()
{
	delete collisionBox;

	delete m_vb;
	delete m_vs;
	delete m_ps;
	delete m_ib;
}

void Cylinder::setPosition(float x, float y, float z)
{
	AGameObject::setPosition(x, y, z);
	collisionBox->setPosition(this->localPosition);
}

void Cylinder::setPosition(Vector3D pos)
{
	AGameObject::setPosition(pos);
	collisionBox->setPosition(this->localPosition);
}

void Cylinder::setScale(float x, float y, float z)
{
	if (x > 0 && y > 0 && z > 0)
	{
		std::vector<Vector3D> newScaleList;

		//revert back
		for (int i = 0; i < edges.size(); i++)
		{
			newScaleList.push_back(Vector3D(edges[i].x / localScale.x, edges[i].y / localScale.y, edges[i].z / localScale.z));
		}


		//apply new scale
		for (size_t i = 0; i < edges.size(); i++)
		{
			newScaleList[i] = Vector3D(newScaleList[i].x * x, newScaleList[i].y * y, newScaleList[i].z * z);
		}

		edges.clear();

		for (size_t i = 0; i < newScaleList.size(); i++)
		{
			edges.push_back(newScaleList[i]);
		}

		AGameObject::setScale(x, y, z);
		collisionBox->setDimensions(1.0f * this->localScale.x, 1.0f * this->localScale.y, 1.0f * this->localScale.z);
	}
}

void Cylinder::setScale(Vector3D newScale)
{


	if (newScale.x > 0 && newScale.y > 0 && newScale.z > 0)
	{
		std::vector<Vector3D> newScaleList;

		//revert back
		for (int i = 0; i < edges.size(); i++)
		{
			newScaleList.push_back(Vector3D(edges[i].x / localScale.x, edges[i].y / localScale.y, edges[i].z / localScale.z));
		}


		//apply new scale
		for (size_t i = 0; i < edges.size(); i++)
		{
			newScaleList[i] = Vector3D(newScaleList[i].x * newScale.x, newScaleList[i].y * newScale.y, newScaleList[i].z * newScale.z);
		}

		edges.clear();

		for (size_t i = 0; i < newScaleList.size(); i++)
		{
			edges.push_back(newScaleList[i]);
		}

		AGameObject::setScale(newScale);
		collisionBox->setDimensions(1.0f * this->localScale.x, 1.0f * this->localScale.y, 1.0f * this->localScale.z);
	}

}

void Cylinder::setColors(Vector3D color)
{
	this->colors = color;
	updateVertexLocations();
}

void Cylinder::setRotation(float x, float y, float z)
{
	AGameObject::setRotation(x, y, z);
	collisionBox->setRotation(Vector3D(x, y, z));
}

void Cylinder::setRotation(Vector3D rotation)
{
	AGameObject::setRotation(rotation);
	collisionBox->setRotation(rotation);
}

void Cylinder::update(float deltaTime)
{
}

void Cylinder::draw(ConstantBuffer* cb)
{
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vs, cb);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_ps, cb);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_ps);

	std::vector<AComponent*> renderComponentList = getComponentsOfType(AComponent::ComponentType::Renderer);
	if (renderComponentList.size() > 0)
	{
		TextureComponent* texComp = (TextureComponent*)renderComponentList[0];
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, texComp->getTexture());
	}
	else
		GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_ps, m_default_tex);


	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::getInstance()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}

std::vector<Vector3D> Cylinder::getVertexWorldPositions()
{
	std::vector<Vector3D> out;
	for (int i = 0; i < edges.size(); i++)
	{
		out.push_back(Quaternion::rotatePointEuler(edges[i], this->localRotation) + this->localPosition);
	}

	AGameObject* nextParent = this->parent;
	while (nextParent != nullptr)
	{
		for (int i = 0; i < out.size(); i++)
		{
			out[i] = Quaternion::rotatePointEuler(out[i], nextParent->getLocalRotation()) + nextParent->getLocalPosition();
		}
		nextParent = nextParent->getParent();
	}

	return out;
}

float Cylinder::checkRaycast(Vector3D rayOrigin, Vector3D rayDirection)
{
	return collisionBox->checkRaycast(rayOrigin, rayDirection);
}

void Cylinder::updateVertexLocations()
{
	RenderSystem* graphEngine = GraphicsEngine::getInstance()->getRenderSystem();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	std::vector<Vector3D> worldLocations = getVertexWorldPositions();

	vertex* vertexList = (vertex*)malloc(sizeof(vertex) * edges.size());

	for (int i = 0; i < edges.size(); i++)
	{
		vertexList[i] = { worldLocations[i], m_texCoords[i] };
	}

	UINT size_list = edges.size();

	
	GraphicsEngine::getInstance()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	delete m_vb;
	m_vb = graphEngine->createVertexBuffer(vertexList, sizeof(vertex), size_list, shader_byte_code, size_shader);
}

void Cylinder::updateVertexLocations(std::vector<Vector3D> edges)
{
	AGameObject::updateVertexLocations();
}

void Cylinder::buildVerticesAndIndices()
{
	// clear memory of prev arrays
	std::vector<float>().swap(m_vertices);
	//std::vector<float>().swap(m_texCoords);
	std::vector<float> texCoords;


	// get unit circle vectors on XY-plane
	std::vector<float> unitVertices = getUnitCircleVertices();

	// put side vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -m_height / 2.0f + i * m_height;           // z value; -h/2 to h/2
		float t = 1.0f - i;                              // vertical tex coord; 1 to 0

		for (int j = 0, k = 0; j <= m_sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uy = unitVertices[k + 1];
			float uz = unitVertices[k + 2];
			// position vector
			m_vertices.push_back(ux * m_radius);            // vx
			m_vertices.push_back(h);					    // vy                       
			m_vertices.push_back(uz * m_radius);			// vz

			// texture coordinate
			texCoords.push_back((float)j / m_sectorCount); // s
			texCoords.push_back(t);                      // t
		}
	}

	// the starting index for the base/top surface
	int baseCenterIndex = (int)m_vertices.size() / 3;
	int topCenterIndex = baseCenterIndex + m_sectorCount + 1; // include center vertex

	// put base and top vertices to arrays
	for (int i = 0; i < 2; ++i)
	{
		float h = -m_height / 2.0f + i * m_height;           // y value; -h/2 to h/2

		// center point
		m_vertices.push_back(0);       m_vertices.push_back(h);		m_vertices.push_back(0);
		texCoords.push_back(0.5f);	   texCoords.push_back(0.5f);

		for (int j = 0, k = 0; j < m_sectorCount; ++j, k += 3)
		{
			float ux = unitVertices[k];
			float uz = unitVertices[k + 2];

			// position vector
			m_vertices.push_back(ux * m_radius);            // vx
			m_vertices.push_back(h);						// vy
			m_vertices.push_back(uz * m_radius);			// vz

			// texture coordinate
			texCoords.push_back(-ux * 0.5f + 0.5f);      // s
			texCoords.push_back(-uz * 0.5f + 0.5f);      // t
		}
	}

	for (int i = 0; i < texCoords.size() / 2; i++)
	{
		m_texCoords.push_back(Vector2D(texCoords[i * 2], texCoords[i * 2 + 1]));
	}


	// generate CCW index list of cylinder triangles
	int k1 = 0;                         // 1st vertex index at base
	int k2 = m_sectorCount + 1;           // 1st vertex index at top

	// indices for the side surface
	for (int i = 0; i < m_sectorCount; ++i, ++k1, ++k2)
	{
		// 2 triangles per sector
		// k1 => k1+1 => k2
		m_indices.push_back(k1);
		m_indices.push_back(k1 + 1);
		m_indices.push_back(k2);

		// k2 => k1+1 => k2+1
		m_indices.push_back(k2);
		m_indices.push_back(k1 + 1);
		m_indices.push_back(k2 + 1);
	}

	// indices for the base surface
	for (int i = 0, k = baseCenterIndex + 1; i < m_sectorCount; ++i, ++k)
	{
		if (i < m_sectorCount - 1)
		{
			m_indices.push_back(baseCenterIndex);
			m_indices.push_back(k + 1);
			m_indices.push_back(k);
		}
		else // last triangle
		{
			m_indices.push_back(baseCenterIndex);
			m_indices.push_back(baseCenterIndex + 1);
			m_indices.push_back(k);
		}
	}

	// indices for the top surface
	for (int i = 0, k = topCenterIndex + 1; i < m_sectorCount; ++i, ++k)
	{
		if (i < m_sectorCount - 1)
		{
			m_indices.push_back(topCenterIndex);
			m_indices.push_back(k);
			m_indices.push_back(k + 1);
		}
		else // last triangle
		{
			m_indices.push_back(topCenterIndex);
			m_indices.push_back(k);
			m_indices.push_back(topCenterIndex + 1);
		}
	}


}

std::vector<float> Cylinder::getUnitCircleVertices()
{
	const float PI = 3.1415926f;
	float sectorStep = 2 * PI / m_sectorCount;
	float sectorAngle;  // radian

	std::vector<float> unitCircleVertices;
	for (int i = 0; i <= m_sectorCount; ++i)
	{
		sectorAngle = i * sectorStep;
		unitCircleVertices.push_back(sin(sectorAngle)); // x
		unitCircleVertices.push_back(0);                // y
		unitCircleVertices.push_back(cos(sectorAngle)); // z
	}
	return unitCircleVertices;
}
