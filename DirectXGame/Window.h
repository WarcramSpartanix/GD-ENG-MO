#pragma once
#include <Windows.h>
#include "EngineTime.h"

class Window
{
public:
	Window();
	~Window();

	bool init();
	bool broadcast();
	bool release();
	bool isRunning();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	int const WIDTH = 1440;
	int const HEIGHT = 900;

protected:
	HWND m_hwnd;
	bool m_is_running;
};

