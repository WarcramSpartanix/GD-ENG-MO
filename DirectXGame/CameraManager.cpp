#include "CameraManager.h"
#include "EngineTime.h"
#include "InputSystem.h"

CameraManager* CameraManager::sharedInstance = nullptr;

CameraManager* CameraManager::getInstance()
{
	if (sharedInstance == nullptr)
 		initialize();
    return sharedInstance;
}

void CameraManager::initialize()
{
    sharedInstance = new CameraManager();
}

void CameraManager::destroy()
{
	sharedInstance->m_active_camera = nullptr;
	delete sharedInstance->m_game_camera;
	delete sharedInstance->m_scene_camera;
}

Camera* CameraManager::getSceneCamera()
{
    return nullptr;
}

Camera** CameraManager::getActiveCameraAddress()
{
	return &m_active_camera;
}

GameCamera* CameraManager::getGameCam()
{
	return m_game_camera;
}

void CameraManager::setGameCamera(GameCamera* gameCamera)
{
	m_game_camera = gameCamera;
}

void CameraManager::setActiveCamera(CameraType type)
{
	switch (type)
	{
	case CameraManager::SCENE_CAMERA:
		if(m_active_camera == m_game_camera)
		{
			m_active_camera = m_scene_camera;
			InputSystem::getInstance()->addListener(m_scene_camera);
			InputSystem::getInstance()->removeListener(m_game_camera);
		}
		break;
	case CameraManager::GAME_CAMERA:
		if (m_active_camera == m_scene_camera && m_game_camera != nullptr)
		{
			m_active_camera = m_game_camera;
			InputSystem::getInstance()->addListener(m_game_camera);
			InputSystem::getInstance()->removeListener(m_scene_camera);
		}
		break;
	default:
		break;
	}

	if (m_active_camera == m_scene_camera && m_game_camera != nullptr)
	{
		m_active_camera = m_game_camera;
		InputSystem::getInstance()->addListener(m_game_camera);
		InputSystem::getInstance()->removeListener(m_scene_camera);
	}
	else
	{
		m_active_camera = m_scene_camera;
		InputSystem::getInstance()->addListener(m_scene_camera);
		InputSystem::getInstance()->removeListener(m_game_camera);
	}
}

void CameraManager::switchCamera()
{
	m_camera_toggle = !m_camera_toggle;
}

void CameraManager::update()
{
	if (m_scene_camera != nullptr)
		m_scene_camera->update(EngineTime::getDeltaTime());
	if (m_game_camera != nullptr)
		m_game_camera->update(EngineTime::getDeltaTime());

	if (m_camera_toggle)
	{
		if (m_active_camera == m_scene_camera && m_game_camera != nullptr)
		{
			m_active_camera = m_game_camera;
			InputSystem::getInstance()->addListener(m_game_camera);
			InputSystem::getInstance()->removeListener(m_scene_camera);
		}
		else 
		{
			m_active_camera = m_scene_camera;
			InputSystem::getInstance()->addListener(m_scene_camera);
			InputSystem::getInstance()->removeListener(m_game_camera);
		}
		m_camera_toggle = false;
	}
}

void CameraManager::drawGameCamera(ConstantBuffer* cb)
{
	if(m_game_camera != nullptr)
		m_game_camera->draw(cb);
	
}

Matrix4x4 CameraManager::getCameraViewMatrix()
{
	return m_scene_camera->getViewMatrix();
}

std::vector<Matrix4x4> CameraManager::getAllCameraViewMatrices()
{
	std::vector<Matrix4x4> out;
	out.push_back(m_scene_camera->getViewMatrix());
	if (m_game_camera != nullptr)
		out.push_back(m_game_camera->getViewMatrix());

	return out;
}

void CameraManager::onKeyDown(int key)
{
}

void CameraManager::onKeyUp(int key)
{


	if (key == 'T')//tab
	{
		m_camera_toggle = !m_camera_toggle;
		
	}
}

void CameraManager::onMouseMove(const Point& delta_mouse_pos)
{
}

void CameraManager::onLeftMouseDown(const Point& mouse_pos)
{
}

void CameraManager::onLeftMouseUp(const Point& mouse_pos)
{
}

void CameraManager::onRightMouseDown(const Point& mouse_pos)
{
}

void CameraManager::onRightMouseUp(const Point& mouse_pos)
{
}

CameraManager::CameraManager()
{
	InputSystem::getInstance()->addListener(this);

	m_scene_camera = new Camera("SceneCamera");
	InputSystem::getInstance()->addListener(m_scene_camera);
	
	/*m_game_camera = new GameCamera("GameCamera", Vector3D(0, 0, -2));
	InputSystem::getInstance()->removeListener(m_game_camera);*/

	m_active_camera = m_scene_camera;

			m_scene_camera->setPosition(0, 0, -2);
}

CameraManager::~CameraManager()
{
}