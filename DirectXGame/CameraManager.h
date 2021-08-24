#pragma once
#include "Camera.h"
#include "GameCamera.h"
#include "InputListener.h"
#include "ConstantBuffer.h"
#include <vector>
class CameraManager: public InputListener
{
public:
	enum CameraType {SCENE_CAMERA, GAME_CAMERA};

	static CameraManager* getInstance();
	static void initialize();
	static void destroy();


	Camera* getSceneCamera();
	Camera** getActiveCameraAddress();
	GameCamera* getGameCam();
	void setGameCamera(GameCamera* gameCamera);
	void setActiveCamera(CameraType type);
	void switchCamera();
	void update();
	void drawGameCamera(ConstantBuffer* cb);
	Matrix4x4 getCameraViewMatrix();

	void alignView();

public:
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& delta_mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

private:
	CameraManager();
	~CameraManager();

	static CameraManager* sharedInstance;

private:
	Camera* m_scene_camera;
	Camera* m_active_camera;
	GameCamera* m_game_camera;
	bool m_camera_toggle = false; // if true, change camera mode

	float m_align_percent = 0.0f;
	bool m_align_animating = false;
};

