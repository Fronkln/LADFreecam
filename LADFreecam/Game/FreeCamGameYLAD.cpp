#include "FreecamGameYLAD.h"
#include "PatternScan.h"
#include "memory.h"

ccamera_set_info FreecamGameYLAD::GetCameraSetFunction()
{
	return (ccamera_set_info)PatternScan("40 57 48 83 EC ? 48 C7 44 24 20 ? ? ? ? 48 89 5C 24 58 48 89 74 24 60 48 8B FA 48 8B F1");
}

void* FreecamGameYLAD::GetPadUpdateFunction()
{
	return PatternScan("0F B6 D1 89 90 60 02 00 00");
}

float* FreecamGameYLAD::GetSpeedValues()
{
	return (float*)resolve_relative_addr((PatternScan(GetModuleHandle(NULL), "80 F9 ? 74 ? 0F B6 C1") + 8), 7);
}

void* FreecamGameYLAD::GetAuthPauseFunction()
{
	return PatternScan("0F BF 93 00 08 00 00 0F B6 C8");
}

void FreecamGameYLAD::enable_no_input(bool enable)
{
	if (pad_input_func == nullptr)
		return;

	if (enable)
	{
		BYTE patch[] = { 0x31, 0xD2, 0x90};
		mem::Patch(pad_input_func, patch, 3);
	}
	else
	{
		BYTE patch[] = { 0x0F, 0xB6, 0xD1 };
		mem::Patch(pad_input_func, patch, 3);
	}
}
void FreecamGameYLAD::pause_auth(bool pause)
{
	if (auth_pause_func == nullptr)
		return;

	if (!pause)
	{
		BYTE patch[] = { 0xF, 0xBF, 0x93, 0x0, 0x8, 0x0, 0x0 };
		mem::Patch((BYTE*)auth_pause_func, patch, 7);
	}
	else
	{
		BYTE patch[] = { 0x31, 0xD2, 0x90, 0x90, 0x90, 0x90, 0x90 };
		mem::Patch((BYTE*)auth_pause_func, patch, 7);
	}
}