#include "FreecamGameY5.h"
#include "PatternScan.h"
#include "memory.h"
#include "Entity/Y5/CCameraBase.h"
#include "Entity/Y5/CActionManager.h"
#include "Entity/Y5/CActionCameraManager.h"

vec4f calc_new_focus_point_y5(float cam_x, float cam_y, float cam_z, float speed_x, float speed_y)
{
	float theta = atan2(cam_z, cam_x) + speed_x;

	float phi = sqrt((pow(cam_x, 2) + pow(cam_z, 2)));
	phi = atan2(phi, cam_y) + speed_y;

	float r = sqrt((pow(cam_x, 2) + pow(cam_y, 2) + pow(cam_z, 2)));

	float r_cam_x = r * cos(theta) * sin(phi);
	float r_cam_z = r * sin(theta) * sin(phi);
	float r_cam_y = r * cos(phi);

	vec4f vec = vec4f();
	vec.x = r_cam_x;
	vec.y = r_cam_y;
	vec.z = r_cam_z;

	return vec;
};


void FreecamGameY5::init()
{
	ActionManager = (CActionManager**)resolve_relative_addr(PatternScan("48 8B 0D ? ? ? ? C5 F8 11 44 24 50"));

	cameraUpdateCall = PatternScan("FF 90 C0 01 00 00 48 8B 03 48 8B CB FF 90 C8 01 00 00");
	noInputFunction = PatternScan("40 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 28");
	noCCCFunction = PatternScan("48 8B C4 57 41 56 41 57 48 81 EC ? ? ? ? 48 C7 40 98 ? ? ? ? 48 89 58 10 48 89 68 18 48 89 70 20 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 80 00 00 00 48 8B E9 C7 81 98 02 00 00 ? ? ? ?");
	memcpy_s(&origCameraUpdateCallBytes, 6, cameraUpdateCall, 6);
}


void FreecamGameY5::on_disable()
{
	mem::Patch((BYTE*)cameraUpdateCall, origCameraUpdateCallBytes, 6);

	uint8_t cccPatch[3] = { 0x48, 0x8B, 0xC4 };
	mem::Patch((BYTE*)noCCCFunction, cccPatch, 3);

	enable_no_input(false);
}

void FreecamGameY5::on_enable()
{
	mem::Nop((BYTE*)cameraUpdateCall, 6);

	CActionManager* actMan = *ActionManager;
	startFov = actMan->actionCameraManager->activeCamera->fov;

	uint8_t cccPatch[3] = {0xC3, 0x90, 0x90};
	mem::Patch((BYTE*)noCCCFunction, cccPatch, 3);

	enable_no_input(true);
}

void FreecamGameY5::update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov)
{
	CActionManager* actMan = *ActionManager;

	CCameraBase* currentCamera = actMan->actionCameraManager->activeCamera;

	if (currentCamera == nullptr)
		return;

	bool paused;

	if (actMan != nullptr)
	{
		paused = actMan->gamePaused;
		enable_no_input(!paused);
	}
	else
		paused = false;

	if (paused)
		return;

	std::cout << std::hex  <<  offsetof(CCameraBase, fov) << std::endl;

	float r_cam_x = currentCamera->focusPos.x - currentCamera->currentPosition.x;
	float r_cam_y = currentCamera->focusPos.y - currentCamera->currentPosition.y;
	float r_cam_z = currentCamera->focusPos.z - currentCamera->currentPosition.z;

	vec4f r;
	r.x = r_cam_x;
	r.y = r_cam_y;
	r.z = r_cam_z;

	vec4f focusOut = calc_new_focus_point_y5(r_cam_x, r_cam_y, r_cam_z, deltaFocusX, deltaFocusY);

	currentCamera->currentPosition.x += r_cam_x * deltaPosY + deltaPosX * r_cam_z;
	currentCamera->currentPosition.y += r_cam_y * deltaPosY;
	currentCamera->currentPosition.z += r_cam_z * deltaPosY - deltaPosX * r_cam_x;

	vec4f newFocus;
	newFocus.x = currentCamera->currentPosition.x + focusOut.x;
	newFocus.y = currentCamera->currentPosition.y + focusOut.y;
	newFocus.z = currentCamera->currentPosition.z + focusOut.z;

	currentCamera->focusPos = newFocus;
	currentCamera->fov = startFov + deltaFov;
}

void FreecamGameY5::enable_no_input(bool enable)
{
	if (!enable)
	{
		BYTE patch[] = { 0x40, 0x57, };
		mem::Patch((BYTE*)noInputFunction, patch, 2);
	}
	else
	{
		BYTE patch[] = { 0xC3, 0x90 };
		mem::Patch((BYTE*)noInputFunction, patch, 2);
	}
}