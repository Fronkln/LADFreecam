#include "FreecamGameLJ.h"
#include "PatternScan.h"
#include "memory.h"

ccamera_set_info FreecamGameLJ::GetCameraSetFunction()
{
	return (ccamera_set_info)PatternScan("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC ? 48 8B FA 48 8B F1 48 81 C1 ? ? ? ? E8 ? ? ? ? C5 F8 10 07");
}

void* FreecamGameLJ::GetPadUpdateFunction()
{
	return PatternScan("48 89 5C 24 18 48 89 6C 24 20 56 57 41 56 48 83 EC ? 48 8B E9 80 3D ? ? ? ? ?");
}

float* FreecamGameLJ::GetSpeedValues()
{
	return (float*)resolve_relative_addr(PatternScan(GetModuleHandle(NULL), "C5 FA 11 15 ? ? ? ? C5 E8 57 D2 C5 EA 2A D0"), 8);
}

void* FreecamGameLJ::GetAuthPauseFunction()
{
	return PatternScan("0F BF 87 10 08 00 00 0F AF 04 8A");
}

void FreecamGameLJ::pause_auth(bool pause)
{
	if (auth_pause_func == nullptr)
		return;

	if (!pause)
	{
		BYTE patch[] = { 0xF, 0xBF, 0x87, 0x10, 0x8, 0x0, 0x0 };
		mem::Patch((BYTE*)auth_pause_func, patch, 7);
	}
	else
	{
		BYTE patch[] = { 0x31, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90 };
		mem::Patch((BYTE*)auth_pause_func, patch, 7);
	}
}