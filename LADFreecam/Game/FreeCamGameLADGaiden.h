#pragma once
#include "FreecamGame.h"

class FreecamGameLADGaiden : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	void* GetPadUpdateFunction();
	float* GetSpeedValues();
	virtual void* GetAuthPauseFunction();
	virtual void pause_auth(bool pause);
};