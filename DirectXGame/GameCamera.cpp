#include "GameCamera.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"
#include "BoundingBox.h"

GameCamera::GameCamera(std::string name, Vector3D pos) : Camera(name)
{
	this->localPosition = pos;
}

GameCamera::~GameCamera()
{
	delete collisionBox;

	m_vb->release();
	m_vs->release();
	m_ps->release();
	m_ib->release();
}

void GameCamera::initializeMesh()
{
	edges[0] = Vector3D(0, 0, 0);
	edges[1] = Vector3D(-0.25f, 0.25f, 0.5f);
	edges[2] = Vector3D(0.25f, -0.25f, 0.5f);
	edges[3] = Vector3D(-0.25f, -0.25f, 0.5f);
	edges[4] = Vector3D(0.25f, 0.25f, 0.5f);

	edges[5] = Vector3D(-0.25f, -0.25f, -0.75f);
	edges[6] = Vector3D(-0.25f, 0.25f, -0.75f);
	edges[7] = Vector3D(0.25f, 0.25f, -0.75f);
	edges[8] = Vector3D(0.25f, -0.25f, -0.75f);
	edges[9] = Vector3D(0.25f, -0.25f, 0);
	edges[10] = Vector3D(0.25f, 0.25f, 0);
	edges[11] = Vector3D(-0.25f, 0.25f, 0);
	edges[12] = Vector3D(-0.25f, -0.25f, 0);

	collisionBox = new BoundingBox(this->localPosition + Vector3D(0,0,-0.125f), this->localRotation, 0.5f, 0.5f, 1.25f);

	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	Vector3D* worldLocations = getVertexWorldPositions();

	vertex list[] = {
		{ worldLocations[0],	Vector3D(1,1,1) },

		{ worldLocations[1],	Vector3D(1,1,1) },
		{ worldLocations[2],	Vector3D(1,1,1) },
		{ worldLocations[3],	Vector3D(1,1,1) },
		{ worldLocations[4],	Vector3D(1,1,1) },

		{ worldLocations[5],	Vector3D(1,1,1) },
		{ worldLocations[6],	Vector3D(1,1,1) },
		{ worldLocations[7],	Vector3D(1,1,1) },
		{ worldLocations[8],	Vector3D(1,1,1) },
		{ worldLocations[9],	Vector3D(1,1,1) },
		{ worldLocations[10],	Vector3D(1,1,1) },
		{ worldLocations[11],	Vector3D(1,1,1) },
		{ worldLocations[12],	Vector3D(1,1,1) },
	};

	m_vb = GraphicsEngine::getInstance()->createVertexBuffer();

	UINT size_list = ARRAYSIZE(list);

	unsigned int index_list[] = {
		3,2,4,
		1,3,4,

		0,4,2,
		0,2,3,
		1,4,0,
		0,3,1,

		5,6,7,
		7,8,5,

		9,10,11,
		11,12,9,

		6,11,10,
		10,7,6,

		12,5,8,
		8,9,12,

		8,7,10,
		10,9,8,

		12,11,6,
		6,5,12
	};

	m_ib = GraphicsEngine::getInstance()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	m_vs = GraphicsEngine::getInstance()->createVertexShader(shader_byte_code, size_shader);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();

	GraphicsEngine::getInstance()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);

	m_ps = GraphicsEngine::getInstance()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();
}

void GameCamera::draw(ConstantBuffer* cb)
{
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_vs, cb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setConstantBuffer(m_ps, cb);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setPixelShader(m_ps);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}

void GameCamera::setPosition(float x, float y, float z)
{
	AGameObject::setPosition(x, y, z);
	collisionBox->setPosition(this->localPosition + Vector3D(0, 0, -0.125f));
}

void GameCamera::setPosition(Vector3D pos)
{
	AGameObject::setPosition(pos);
	collisionBox->setPosition(this->localPosition + Vector3D(0, 0, -0.125f));
}

void GameCamera::setScale(float x, float y, float z)
{
}

void GameCamera::setScale(Vector3D scale)
{
}

void GameCamera::setRotation(float x, float y, float z)
{
	AGameObject::setRotation(x, y, z);
	collisionBox->setRotation(Vector3D(x, y, z));
}

void GameCamera::setRotation(Vector3D rot)
{
	AGameObject::setRotation(rot);
	collisionBox->setRotation(rot);
}

Vector3D* GameCamera::getVertexWorldPositions()
{
	Vector3D worldLocations[] = {
		Quaternion::rotatePointEuler(edges[0], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[1], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[2], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[3], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[4], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[5], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[6], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[7], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[8], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[9], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[10], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[11], this->localRotation) + this->localPosition,
		Quaternion::rotatePointEuler(edges[12], this->localRotation) + this->localPosition,
	};

	return worldLocations;
}

float GameCamera::checkRaycast(Vector3D rayOrigin, Vector3D rayDirection)
{
	return collisionBox->checkRaycast(rayOrigin, rayDirection);
}

void GameCamera::updateVertexLocations()
{
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	Vector3D* worldLocations = getVertexWorldPositions();

	vertex list[] = {
		{ worldLocations[0],	Vector3D(1,1,1) },

		{ worldLocations[1],	Vector3D(1,1,1) },
		{ worldLocations[2],	Vector3D(1,1,1) },
		{ worldLocations[3],	Vector3D(1,1,1) },
		{ worldLocations[4],	Vector3D(1,1,1) },
		
		{ worldLocations[5],	Vector3D(1,1,1) },
		{ worldLocations[6],	Vector3D(1,1,1) },
		{ worldLocations[7],	Vector3D(1,1,1) },
		{ worldLocations[8],	Vector3D(1,1,1) },
		{ worldLocations[9],	Vector3D(1,1,1) },
		{ worldLocations[10],	Vector3D(1,1,1) },
		{ worldLocations[11],	Vector3D(1,1,1) },
		{ worldLocations[12],	Vector3D(1,1,1) },
	};

	UINT size_list = ARRAYSIZE(list);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();
}
