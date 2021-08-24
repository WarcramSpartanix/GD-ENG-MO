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

	//align with view
	bool ctrl = InputSystem::getInstance()->isKeyDown(16);
	bool shift = InputSystem::getInstance()->isKeyDown(17);
	bool F = InputSystem::getInstance()->isKeyDown(70);
	if (ctrl && shift && F)
	{
		m_align_animating = true;
	}
	if (m_game_camera != nullptr && m_align_animating == true)
	{
		if (m_align_percent < 1.0f)
		{
			m_align_percent += EngineTime::getDeltaTime();
			m_game_camera->setPosition(Vector3D::lerp(m_game_camera->getLocalPosition(), m_scene_camera->getLocalPosition(), m_align_percent));
			m_game_camera->setRotation(Vector3D::lerp(m_game_camera->getLocalRotation(), m_scene_camera->getLocalRotation(), m_align_percent));
		}
		else
		{
			m_align_percent = 0.0f;
			m_align_animating = false;
		}
	}
	else
		m_align_animating = false; // in case game camera not yet created. Avoid issues

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

void CameraManager::alignView()
{
	m_align_animating = true;
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