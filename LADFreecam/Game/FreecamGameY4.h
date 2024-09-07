#pragma once
#include "FreecamGameY3.h"

class CActionCameraManager;

class FreecamGameY4 : public FreecamGameY3
{
	virtual void on_disable();
	virtual void on_enable();
	virtual void update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov);
	virtual void* GetPadUpdateFunction();
	virtual UINT8** get_action_manager();
	virtual CActionCameraManager** get_camera_manager();
	virtual _DoCameraTransition get_camera_transition_func();
	virtual void* get_no_ccc_func();
};