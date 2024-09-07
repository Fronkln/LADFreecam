#include "FreecamGameY6.h"
#include "PatternScan.h"
#include "memory.h"

ccamera_set_info FreecamGameY6::GetCameraSetFunction()
{
	ccamera_set_info pat1 = (ccamera_set_info)PatternScan("40 57 48 83 EC ? 48 C7 44 24 20 ? ? ? ? 48 89 5C 24 60 48 89 74 24 68 48 8B FA 48 8B F1 48 8D 99 A0 00 00 00 48 89 5C 24 58 48 8B CB E8 ? ? ? ? 90 C5 F8 10 17");

	if (pat1 == nullptr)
	{
		//y6 steam pattern here
	}

	return pat1;
}

void* FreecamGameY6::GetPadUpdateFunction()
{
	return PatternScan("48 89 5C 24 08 57 48 81 EC ? ? ? ? 44 8B 89 E0 00 00 00");
}

void FreecamGameY6::enable_no_input(bool enable)
{
	if (!enable)
	{
		BYTE patch[] = { 0x48, 0x89, 0x5C, 0x24, 0x8 };
		mem::Patch((BYTE*)pad_input_func, patch, 5);
	}
	else
	{
		BYTE patch[] = { 0xC3, 0x90, 0x90, 0x90, 0x90 };
		mem::Patch((BYTE*)pad_input_func, patch, 5);
	}
}