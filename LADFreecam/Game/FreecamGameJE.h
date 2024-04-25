#pragma once
#include "FreecamGame.h"

class FreecamGameJE : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	void* GetPadUpdateFunction();
	void enable_no_input(bool enable);
	float* GetSpeedValues();
};