#pragma once
#include "FreecamGame.h"

class FreecamGameYLAD : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	void* GetPadUpdateFunction();
	virtual float* GetSpeedValues();
	virtual void* GetAuthPauseFunction();
	void enable_no_input(bool enable);
	virtual void pause_auth(bool pause);
};