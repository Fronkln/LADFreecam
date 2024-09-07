#include "types.h"
#include <cstdint>

#pragma pack(push, 1)
class CCameraBase
{
	uint8_t baseclass[320];

public:
	int32_t cameraIdx; //0x0140
	void* N0000224E; //0x0144
	char pad_014C[4]; //0x014C
	vec4f currentPosition; //0x0150
	vec4f focusPos; //0x0160
	char pad_0170[144]; //0x0170
	float nearClip; //0x0200
	float farClip; //0x0204
	float fov; //0x0208
	char pad_020C[180]; //0x020C
}; //Size: 0x02C0
#pragma pack(pop)