#include "types.h"
#include <cstdint>

#pragma pack(push, 1)
class CCameraBase
{
	uint8_t baseclass[160];

public:
	uint32_t cameraIndex; //0x00A0
	float nearClip; //0x00A4
	float farClip; //0x00A8
	float fov; //0x00AC
	char pad_00B0[48]; //0x00B0
	vec4f currentPosition; //0x00E0
	vec4f focusPos; //0x00F0
}; //Size: 0x02C0
#pragma pack(pop)