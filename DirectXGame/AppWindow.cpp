#include "AppWindow.h"
#include <Windows.h>
#include "GameObjectConstants.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include <iostream>
#include <random>
#include "UIManager.h"
#include "GameObjectManager.h"
#include "ViewportScreen.h"

AppWindow* AppWindow::sharedInstance = nullptr;

AppWindow::AppWindow()
{
	
}

AppWindow::~AppWindow()
{
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	if (*CameraManager::getInstance()->getActiveCameraAddress() == CameraManager::getInstance()->getGameCam())
		return;

	POINT point = { mouse_pos.x, mouse_pos.y };
	ScreenToClient(this->m_hwnd, &point);

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	Vector3D convertedPos = Vector3D((float)point.x / (float)width * 2.0f - 1.0f, -(float)point.y / (float)height * 2.0f + 1.0f, 1.0f);

	Matrix4x4 inverseProj(cc.m_proj);
	inverseProj.invert();
	Vector3D raycastEye = inverseProj * convertedPos;
	raycastEye.z = 1.0f;

	Vector3D raycastWorld;
	if (isPerspective)
	{
		raycastWorld = Quaternion::rotatePointEuler(raycastEye, (*cam)->getLocalRotation());
		raycastWorld.normalize();//normalize if we want direction vector for perspective raycast
	}
	else
	{
		raycastWorld = Quaternion::rotatePointEuler(raycastEye, (*cam)->getLocalRotation());
	}

	//find if any object collides with the raycast and get the one that is the closest to the raycast origin
	float minT = INT_MAX;
	int minIndex = -1;
	float t = 0;
	//ortho raycast comes from cursor straight forward along z
	//perspective raycast comes from camera position in the direction of raycast world
	for (int i = 0; i < cubes.size(); i++)
	{
		if (!isPerspective) 
			t = cubes[i]->checkRaycast(raycastWorld + (*cam)->getLocalPosition() + (*cam)->getForwardVector() * (orthoNearPlane), (*cam)->getForwardVector());
		else 
			t = cubes[i]->checkRaycast((*cam)->getLocalPosition(), raycastWorld);

		if (t != -9999)
		{
			if (t < minT)
			{
				minT = t;
				minIndex = i;
				//cubes[i]->setColors(Vector3D(0, 0, 1));
			}
		}
	}
	//check with cam mesh
	if (!isPerspective)
		t = CameraManager::getInstance()->getGameCam()->checkRaycast(raycastWorld + (*cam)->getLocalPosition() + (*cam)->getForwardVector() * (orthoNearPlane), (*cam)->getForwardVector());
	else
	{
		if(CameraManager::getInstance()->getGameCam() != nullptr)
			t = CameraManager::getInstance()->getGameCam()->checkRaycast((*cam)->getLocalPosition(), raycastWorld);
	}

	if (t != -9999)
	{
		if (t < minT)
		{
			minT = t;
			minIndex = cubes.size();
		}
	}

	if (minIndex != -1)
	{
		if (minIndex < cubes.size())
			selectedCube = cubes[minIndex];
		else
			selectedCam = CameraManager::getInstance()->getGameCam();
	}
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	selectedCube = nullptr;
	selectedCam = nullptr;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	
}

void AppWindow::onMouseMove(const Point& delta_mouse_pos)
{
	/*RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	rot_x += (mouse_pos.y - height / 2.0f) * m_delta_time * 0.25f;
	rot_y += (mouse_pos.x - width / 2.0f) * m_delta_time * 0.25f;

	InputSystem::getInstance()->setCursorPosition(Point(width / 2.0f, height / 2.0f));*/
	if (selectedCube != nullptr)
	{
		Vector3D cubePos = selectedCube->getLocalPosition();
		Matrix4x4 viewMat = (*cam)->getViewMatrix();
		Vector3D newPos = cubePos + viewMat.getXDirection() * delta_mouse_pos.x * 0.5f * m_delta_time - 
							viewMat.getYDirection() * delta_mouse_pos.y * 0.5f * m_delta_time;
		selectedCube->setPosition(newPos);
	}
	else if (selectedCam != nullptr)
	{
		Vector3D cubePos = selectedCam->getLocalPosition();
		Matrix4x4 viewMat = (*cam)->getViewMatrix();
		Vector3D newPos = cubePos + viewMat.getXDirection() * delta_mouse_pos.x * 0.5f * m_delta_time -
			viewMat.getYDirection() * delta_mouse_pos.y * 0.5f * m_delta_time;
		selectedCam->setPosition(newPos);
	}
}

void AppWindow::onKeyDown(int key)
{
	if (key == VK_ESCAPE)
	{
		
	}
}

void AppWindow::onKeyUp(int key)
{

}

AppWindow* AppWindow::getInstance()
{
	if (sharedInstance == nullptr)
		AppWindow::initialize();
	return sharedInstance;
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
	sharedInstance->init();
}

void AppWindow::destroy()
{
	if (sharedInstance != nullptr)
		sharedInstance->onDestroy();
}

void AppWindow::update()
{
	CameraManager::getInstance()->update();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	m_delta_scale += m_delta_time / 0.5f;

	Matrix4x4 temp;

	cc.m_world.setIdentity();

	cc.m_view = (*cam)->getViewMatrix();
	cc.m_view.invert();

	m_cb->update(GraphicsEngine::getInstance()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();

	GameObjectManager::initialize();
	InputSystem::getInstance()->addListener(this);
	cam = CameraManager::getInstance()->getActiveCameraAddress();
	//InputSystem::getInstance()->showCursor(false);

	cc.m_view.setIdentity();

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	if (!isPerspective)
		cc.m_proj.setOrthoLH(width / 400.0f, height / 400.0f, orthoNearPlane, 4.0f);
	else
		cc.m_proj.setPerspectiveFovLH(1.57f, (float)width / (float)height, 0.1f, 100.0f);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::getInstance()->update();

	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	GraphicsEngine::getInstance()->getImmediateDeviceContext()->setViewportSize(width, height);

	update();

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(renderToTex->getRenderTargetView(), this->m_swap_chain->m_dsv, 0, 0.3f, 0.4f, 1);

	GameObjectManager::getInstance()->updateAll();
	GameObjectManager::getInstance()->renderAll(m_cb);

	for (int i = 0; i < cubes.size(); i++)
	{
		this->cubes[i]->update(m_delta_time);
		this->cubes[i]->draw(m_cb);
	}
	CameraManager::getInstance()->drawGameCamera(m_cb);

	UIManager::getInstance()->getViewportUI()->setTex(renderToTex->getShaderResourceView());

	GraphicsEngine::getInstance()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0.3f, 0.4f, 1);
	
	GameObjectManager::getInstance()->updateAll();
	GameObjectManager::getInstance()->renderAll(m_cb);

	for (int i = 0; i < cubes.size(); i++)
	{
		this->cubes[i]->update(m_delta_time);
		this->cubes[i]->draw(m_cb);
	}
	CameraManager::getInstance()->drawGameCamera(m_cb);
	//plane->draw(m_cb);

	UIManager::getInstance()->drawAllUI();

	m_swap_chain->present(true);

	m_previous_time = m_current_time;
	m_current_time = ::GetTickCount();
	m_delta_time = EngineTime::getDeltaTime();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	renderToTex->Destroy();
	m_swap_chain->release();
	CameraManager::getInstance()->destroy();
	GameObjectManager::getInstance()->destroy();
	GraphicsEngine::getInstance()->destroy();
}

void AppWindow::onFocus()
{
	InputSystem::getInstance()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::getInstance()->removeListener(this);
}

void AppWindow::createGraphicsWindow()
{
	GraphicsEngine::intialize();
	GraphicsEngine* graphEngine = GraphicsEngine::getInstance();

	m_swap_chain = GraphicsEngine::getInstance()->createSwapChain();
	RECT rc = this->getClientWindowRect();
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	m_swap_chain->init(this->m_hwnd, width, height);
	for (int i = 0; i < 3; i++)
	{
		Vector3D loc = Vector3D(rand() % 200 / 100.0f - 1.0f, rand() % 200 / 100.0f - 1.0f, rand() % 200 / 100.0f - 1.0f);
		Cube* cubey = new Cube("Cube " + std::to_string(i), loc, Vector3D(1,1,1), Vector3D(0, 1, 1), Vector3D());
		//this->cubes.push_back(cubey);

		GameObjectManager::getInstance()->addObject(cubey);
	}
	//CameraManager::getInstance()->getGameCam()->initializeMesh();
	//plane = new Plane("Plane", Vector3D(0, -0.25f, 0), Vector3D(3, 1, 3), Vector3D(1, 1, 0), Vector3D(0,0,0));
	UIManager::initialize(this->m_hwnd);

	renderToTex = new RenderToTexture();
	renderToTex->Initialize(width, height);

	cc.m_time = 0;

	m_cb = GraphicsEngine::getInstance()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::initializeEngine()
{
}
