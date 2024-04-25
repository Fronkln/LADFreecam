#pragma once
#include "FreecamGame.h"

class FreecamGameLADIW : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	void* GetPadUpdateFunction();
	virtual void* GetAuthPauseFunction();
	float* GetSpeedValues();
	virtual void pause_auth(bool pause);
};