#pragma once
#include "types.h"
#include <Windows.h>

class FreecamGame
{
public:
	BYTE* pad_input_func = nullptr;
	void* auth_pause_func = nullptr;
	__int8 noInput_originalByte = 0;

public:
	virtual ccamera_set_info GetCameraSetFunction();
	virtual void* GetPadUpdateFunction();
	virtual float* GetSpeedValues();
	virtual void* GetAuthPauseFunction();
	virtual void enable_no_input(bool enable);
	virtual void pause_auth(bool pause);
};