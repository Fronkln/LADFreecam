#pragma once
#include "FreecamGame.h"

class FreecamGameY6 : public FreecamGame
{
public:
	ccamera_set_info GetCameraSetFunction();
	virtual void* GetPadUpdateFunction();
	virtual void enable_no_input(bool enable);

	virtual void* GetAuthPauseFunction();
	virtual void pause_auth(bool pause);

	bool isSteam;

	void* pauseAuthFunc1;
	void* pauseAuthFunc2;
};