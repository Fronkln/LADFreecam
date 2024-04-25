#include "FreecamGameJE.h"
#include "PatternScan.h"
#include "memory.h"

ccamera_set_info FreecamGameJE::GetCameraSetFunction()
{
	return (ccamera_set_info)PatternScan("40 57 48 83 EC ? 48 C7 44 24 20 ? ? ? ? 48 89 5C 24 58 48 89 74 24 60 48 8B FA 48 8B F1");
}

float* FreecamGameJE::GetSpeedValues()
{
	return (float*)resolve_relative_addr(PatternScan(GetModuleHandle(NULL), "C5 FA 11 15 ? ? ? ? C5 E8 57 D2 C5 EA 2A D0"), 8);
}

void* FreecamGameJE::GetPadUpdateFunction()
{
	return PatternScan("0F B6 D1 89 90 60 02 00 00");
}

void FreecamGameJE::enable_no_input(bool enable)
{
	if (pad_input_func == nullptr)
		return;

	if (!enable)
	{
		BYTE patch[] = { 0xF, 0xB6, 0xD1 };
		mem::Patch((BYTE*)pad_input_func, patch, 3);
	}
	else
	{
		BYTE patch[] = { 0x31, 0xD2, 0x90};
		mem::Patch((BYTE*)pad_input_func, patch, 3);
	}
}