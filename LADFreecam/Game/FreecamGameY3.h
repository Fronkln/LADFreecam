#pragma once
#include "FreecamGame.h"

class CActionCameraManager;

class FreecamGameY3 : public FreecamGame
{
	void* noInputFunction;
	CActionCameraManager** cameraManager;
	int lastCam = 0;
	float startFov;

	virtual void init();
	virtual void on_disable();
	virtual void on_enable();
	virtual void enable_no_input(bool enable);
	virtual void update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov);
};