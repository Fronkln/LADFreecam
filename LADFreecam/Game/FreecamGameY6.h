#pragma once
#include "FreecamGame.h"

class FreecamGameY6 : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	virtual void* GetPadUpdateFunction();
	virtual void enable_no_input(bool enable);
};