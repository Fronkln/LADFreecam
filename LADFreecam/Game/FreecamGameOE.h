#include "FreecamGame.h"
#include <cstdint>

class FreecamGameOE : public FreecamGame
{
public:
	class CActionManager** ActionManager;
	class CActionCameraManager** ActionCameraManager;

	void* cameraUpdateCall;
	void* noInputFunction;
	void* noCCCFunction;
	uint8_t origCameraUpdateCallBytes[6];

	int lastCamId;
	float startFov;

	bool isY0 = false;

	virtual void init();
	virtual void on_disable();
	virtual void on_enable();
	virtual void update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov);
	virtual void enable_no_input(bool enable);
};