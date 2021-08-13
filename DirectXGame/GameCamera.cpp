#include "GameCamera.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "DeviceContext.h"
#include "IndexBuffer.h"

GameCamera::GameCamera(std::string name, Vector3D pos) : Camera(name)
{
	this->localPosition = pos;
}

GameCamera::~GameCamera()
{
	m_vb->release();
	m_vs->release();
	m_ps->release();
	m_ib->release();
}

void GameCamera::initializeMesh()
{
	edges[0] = Vector3D(-this->localScale.x / 2.0f, -this->localScale.y / 2.0f, -this->localScale.z / 2.0f);
	edges[1] = Vector3D(-this->localScale.x / 2.0f, this->localScale.y / 2.0f, -this->localScale.z / 2.0f);
	edges[2] = Vector3D(this->localScale.x / 2.0f, this->localScale.y / 2.0f, -this->localScale.z / 2.0f);
	edges[3] = Vector3D(this->localScale.x / 2.0f, -this->localScale.y / 2.0f, -this->localScale.z / 2.0f);
	edges[4] = Vector3D(this->localScale.x / 2.0f, -this->localScale.y / 2.0f, this->localScale.z / 2.0f);
	edges[5] = Vector3D(this->localScale.x / 2.0f, this->localScale.y / 2.0f, this->localScale.z / 2.0f);
	edges[6] = Vector3D(-this->localScale.x / 2.0f, this->localScale.y / 2.0f, this->localScale.z / 2.0f);
	edges[7] = Vector3D(-this->localScale.x / 2.0f, -this->localScale.y / 2.0f, this->localScale.z / 2.0f);

	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	Vector3D* worldLocations = getVertexWorldPositions();

	vertex list[] = {
		{ worldLocations[0],		Vector3D(1,1,1) },
		{ worldLocations[1],		Vector3D(1,1,1) },
		{ worldLocations[2],		Vector3D(1,1,1) },
		{ worldLocations[3],		Vector3D(1,1,1) },

		{ worldLocations[4],		Vector3D(1,1,1) },
		{ worldLocations[5],		Vector3D(1,1,1) },
		{ worldLocations[6],		Vector3D(1,1,1) },
		{ worldLocations[7],		Vector3D(1,1,1) },
	};

	m_vb = GraphicsEngine::getInstance()->createVertexBuffer();

	UINT size_list = ARRAYSIZE(list);

	unsigned int index_list[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,6,5,
		5,2,1,

		7,0,3,
		3,4,7,

		3,2,5,
		5,4,3,

		7,6,1,
		1,0,7
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
	};

	return worldLocations;
}

void GameCamera::updateVertexLocations()
{
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	graphEngine->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	Vector3D* worldLocations = getVertexWorldPositions();

	vertex list[] = {
		{ worldLocations[0],		Vector3D(1,1,1) },
		{ worldLocations[1],		Vector3D(1,1,1) },
		{ worldLocations[2],		Vector3D(1,1,1) },
		{ worldLocations[3],		Vector3D(1,1,1) },

		{ worldLocations[4],		Vector3D(1,1,1) },
		{ worldLocations[5],		Vector3D(1,1,1) },
		{ worldLocations[6],		Vector3D(1,1,1) },
		{ worldLocations[7],		Vector3D(1,1,1) },
	};

	UINT size_list = ARRAYSIZE(list);

	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::getInstance()->releaseCompiledShader();
}
