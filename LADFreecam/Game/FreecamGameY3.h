#pragma once
#include "FreecamGame.h"
#include <cstdint>

class CActionCameraManager;

class FreecamGameY3 : public FreecamGame
{
public:
	typedef void(__fastcall* _DoCameraTransition)(void* thisPtr, unsigned int var1, int var2);

	void* noInputFunction;
	void* noCCCFunction;
	CActionCameraManager** cameraManager;
	UINT8** actionManager;
	int lastCam = 0;
	float startFov;

	_DoCameraTransition _cameraFunc;


	virtual void init();
	virtual void on_disable();
	virtual void on_enable();
	virtual void enable_no_input(bool enable);
	virtual void update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov);
	virtual void* GetPadUpdateFunction();
	virtual UINT8** get_action_manager();
	virtual CActionCameraManager** get_camera_manager();
	virtual _DoCameraTransition get_camera_transition_func();
	virtual void* GetAuthPauseFunction();
	virtual void pause_auth(bool pause);
	virtual void* get_no_ccc_func();
};