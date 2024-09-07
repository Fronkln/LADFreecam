#include "FreecamGameY4.h"
#include "..\Entity\Y3\CActionCameraManager.h"
#include "..\Entity\Y4\CCameraBase.h"
#include "PatternScan.h"
#include "memory.h"

vec4f calc_new_focus_point_y4(float cam_x, float cam_y, float cam_z, float speed_x, float speed_y)
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


void* FreecamGameY4::GetPadUpdateFunction() 
{
	return PatternScan("89 4C 24 08 53 56 57 48 81 EC ? ? ? ?");
}

CActionCameraManager** FreecamGameY4::get_camera_manager()
{
	return (CActionCameraManager**)(resolve_relative_addr(PatternScan("48 8B 0D ? ? ? ? E8 ? ? ? ? 41 89 86 C0 1A 00 00")));
}

FreecamGameY3::_DoCameraTransition FreecamGameY4::get_camera_transition_func() 
{
	return (FreecamGameY3::_DoCameraTransition)PatternScan("48 89 5C 24 10 48 89 6C 24 18 57 48 83 EC ? 41 8B E8");
}

byte** FreecamGameY4::get_action_manager()
{
	return (byte**)resolve_relative_addr(PatternScan("48 8B 0D ? ? ? ? E8 ? ? ? ? 48 63 44 24 60"));
}

void* FreecamGameY4::get_no_ccc_func()
{
	return PatternScan("40 55 56 57 41 56 41 57 48 83 EC ? 48 C7 44 24 20 ? ? ? ? 48 89 9C 24 A0 00 00 00");
}

void FreecamGameY4::on_disable()
{
	CActionCameraManager* mng = *cameraManager;

	if (mng == nullptr)
		return;

	if (mng->numCameras < lastCam)
		lastCam = 2;

	_cameraFunc(mng, lastCam, 0);
	enable_no_input(false);

	if (noCCCFunction != nullptr)
	{
		BYTE patch[] = { 0x40, 0x55 };
		mem::Patch((BYTE*)noCCCFunction, patch, 2);
	}
}

void FreecamGameY4::on_enable()
{
	CActionCameraManager* mng = *cameraManager;

	if (mng == nullptr)
		return;

	lastCam = mng->activeCameraID;
	CCameraBase* lastCamObject = (CCameraBase*)mng->currentCameraPtr;

	//rarely nullptr
	if (lastCamObject != nullptr)
	{
		CCameraBase* freecamCam = mng->cameraArray[1];

		freecamCam->currentCameraPosition = lastCamObject->currentCameraPosition;
		freecamCam->focusPos = lastCamObject->focusPos;
		freecamCam->fov = lastCamObject->fov;
		startFov = freecamCam->fov;
	}

	_cameraFunc(mng, 1, 0);
	enable_no_input(true);

	if (noCCCFunction != nullptr)
	{
		BYTE patch[] = { 0xC3, 0x90 };
		mem::Patch((BYTE*)noCCCFunction, patch, 2);
	}
}

void FreecamGameY4::update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov)
{
	CActionCameraManager* mng = *cameraManager;

	if (mng == nullptr)
		return;

	CCameraBase* currentCamera = mng->cameraArray[1];

	float r_cam_x = currentCamera->focusPos.x - currentCamera->currentCameraPosition.x;
	float r_cam_y = currentCamera->focusPos.y - currentCamera->currentCameraPosition.y;
	float r_cam_z = currentCamera->focusPos.z - currentCamera->currentCameraPosition.z;

	vec4f r;
	r.x = r_cam_x;
	r.y = r_cam_y;
	r.z = r_cam_z;

	vec4f focusOut = calc_new_focus_point_y4(r_cam_x, r_cam_y, r_cam_z, deltaFocusX, deltaFocusY);

	currentCamera->currentCameraPosition.x += r_cam_x * deltaPosY + deltaPosX * r_cam_z;
	currentCamera->currentCameraPosition.y += r_cam_y * deltaPosY;
	currentCamera->currentCameraPosition.z += r_cam_z * deltaPosY - deltaPosX * r_cam_x;

	vec4f newFocus;
	newFocus.x = currentCamera->currentCameraPosition.x + focusOut.x;
	newFocus.y = currentCamera->currentCameraPosition.y + focusOut.y;
	newFocus.z = currentCamera->currentCameraPosition.z + focusOut.z;

	currentCamera->focusPos = newFocus;
	currentCamera->fov = startFov + deltaFov;

	if (actionManager != nullptr)
	{
		void* actionMan = *actionManager;

		if (actionMan != nullptr)
		{
			bool* paused = (bool*)((byte*)actionMan + 0x10);
			enable_no_input(!(*paused));
		}
	}
}
