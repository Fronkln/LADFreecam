// Created with ReClass.NET 1.2 by KN4CK3R
#include <cstdint>
#include "types.h"

class Entity
{
	byte dat[384];
};

class CCameraBase : public Entity
{
public:
	uint32_t N00007217; //0x0180
	char pad_0184[4]; //0x0184
	float N00007218; //0x0188
	float N000041B1; //0x018C
	vec4f currentCameraPosition; //0x0190
	vec4f focusPos; //0x01A0
	char pad_01B0[8]; //0x01B0
	float N000041B6; //0x01B8
	char pad_01BC[4]; //0x01BC
	vec3f N00008C34; //0x01C0
	char pad_01CC[8]; //0x01CC
	matrix4x3 N00007273; //0x01D4
	char pad_0204[16]; //0x0204
	vec3f realCameraPos; //0x0214
	char pad_0220[8]; //0x0220
	float N00008C19; //0x0228
	float N0000726E; //0x022C
	char pad_0230[8]; //0x0230
	float N00008C1D; //0x0238
	char pad_023C[4]; //0x023C
	float nearDistClip; //0x0240
	float farDistClip; //0x0244
	float fov; //0x0248
	float N00005628; //0x024C
	float N0000562A; //0x0250
	char pad_0254[24]; //0x0254
	int64_t N0000722A; //0x026C
	char pad_0274[52]; //0x0274
}; //Size: 0x02A8