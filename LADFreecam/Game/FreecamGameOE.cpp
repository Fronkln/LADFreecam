#include "FreecamGameOE.h"
#include "PatternScan.h"
#include "memory.h"
#include "Entity/OE/CCameraBase.h"
#include "Entity/OE/CActionCameraManager.h"

vec4f calc_new_focus_point_oe(float cam_x, float cam_y, float cam_z, float speed_x, float speed_y)
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


void FreecamGameOE::init()
{
	ActionCameraManager = (CActionCameraManager**)resolve_relative_addr(PatternScan("48 8B 0D ? ? ? ? E8 ? ? ? ? 0F 57 D2 BA ? ? ? ? 48 8D 4C 24 40"));

	cameraUpdateCall = PatternScan("FF 90 ? ? ? ? 48 8B CB E8 ? ? ? ? 48 8B 03 48 8B CB FF 90");
	noInputFunction = PatternScan("48 89 5C 24 08 48 89 74 24 10 57 48 81 EC ? ? ? ? 48 8B D9 8B 89 E0 00 00 00");

	if (noInputFunction == nullptr)
	{
		noInputFunction = PatternScan("40 53 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 50 01 00 00");
		isY0 = true;
	}

	noCCCFunction = PatternScan("48 8B C4 57 41 56 41 57 48 81 EC ? ? ? ? 48 C7 40 98 ? ? ? ? 48 89 58 10 48 89 68 18 48 89 70 20 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 80 00 00 00 48 8B E9 C7 81 98 02 00 00 ? ? ? ?");
}


void FreecamGameOE::on_disable()
{
	mem::Patch((BYTE*)cameraUpdateCall, origCameraUpdateCallBytes, 6);

	if (noCCCFunction != nullptr)
	{
		uint8_t cccPatch[3] = { 0x48, 0x8B, 0xC4 };
		mem::Patch((BYTE*)noCCCFunction, cccPatch, 3);
	}

	enable_no_input(false);
}

void FreecamGameOE::on_enable()
{
	memcpy_s(&origCameraUpdateCallBytes, 6, cameraUpdateCall, 6);

	mem::Nop((BYTE*)cameraUpdateCall, 6);

	CActionCameraManager* cameraManager = *ActionCameraManager;
	startFov = cameraManager->activeCamera->fov;

	if (noCCCFunction != nullptr)
	{
		uint8_t cccPatch[3] = { 0xC3, 0x90, 0x90 };
		mem::Patch((BYTE*)noCCCFunction, cccPatch, 3);
	}

	enable_no_input(true);
}

void FreecamGameOE::update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov)
{
	CActionCameraManager* cameraManager = *ActionCameraManager;
	CCameraBase* currentCamera = cameraManager->activeCamera;

	if (currentCamera == nullptr)
		return;

	bool paused = false;

	CActionManager* actMan = nullptr;

	if (actMan != nullptr)
	{
		//spaused = actMan->gamePaused;
		enable_no_input(!paused);
	}
	else
		paused = false;

	if (paused)
		return;

	float r_cam_x = currentCamera->focusPos.x - currentCamera->currentPosition.x;
	float r_cam_y = currentCamera->focusPos.y - currentCamera->currentPosition.y;
	float r_cam_z = currentCamera->focusPos.z - currentCamera->currentPosition.z;

	vec4f r;
	r.x = r_cam_x;
	r.y = r_cam_y;
	r.z = r_cam_z;

	vec4f focusOut = calc_new_focus_point_oe(r_cam_x, r_cam_y, r_cam_z, deltaFocusX, deltaFocusY);

	currentCamera->currentPosition.x += r_cam_x * deltaPosY + deltaPosX * r_cam_z;
	currentCamera->currentPosition.y += r_cam_y * deltaPosY;
	currentCamera->currentPosition.z += r_cam_z * deltaPosY - deltaPosX * r_cam_x;

	vec4f newFocus;
	newFocus.x = currentCamera->currentPosition.x + focusOut.x;
	newFocus.y = currentCamera->currentPosition.y + focusOut.y;
	newFocus.z = currentCamera->currentPosition.z + focusOut.z;

	vec4f* focusPos = (vec4f*)((unsigned long long)currentCamera + 0xF0);

	*focusPos = newFocus;
	currentCamera->fov = startFov + deltaFov;
}

void FreecamGameOE::enable_no_input(bool enable)
{
	if (!enable)
	{
		if (isY0)
		{
			BYTE patch[] = { 0x40, 0x53, };
			mem::Patch((BYTE*)noInputFunction, patch, 2);
		}
		else
		{
			BYTE patch[] = { 0x48, 0x89, };
			mem::Patch((BYTE*)noInputFunction, patch, 2);
		}
	}
	else
	{
		BYTE patch[] = { 0xC3, 0x90 };
		mem::Patch((BYTE*)noInputFunction, patch, 2);
	}
}