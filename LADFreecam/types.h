#pragma once
#include <iostream>;
#include <fstream>;
#include "GLM/glm.hpp"


class vec3f
{
public:
	float x;
	float y;
	float z;

	friend std::ostream& operator<<(std::ostream& os, const vec3f& vec)
	{
		os << "{" << vec.x << " " << vec.y << " " << vec.z << "}";
		return os;
	};
};

class vec4f
{
public:
	float x;
	float y;
	float z;
	float w;

	friend std::ostream& operator<<(std::ostream& os, const vec4f& vec)
	{
		os << "{" << vec.x << " " << vec.y << " " << vec.z << "}";
		return os;
	};
};

class matrix4x3
{
	vec4f vm0;
	vec4f vm1;
	vec4f vm2;
};

class camera_info
{
public:
	glm::vec4 pos;
	glm::vec4 focus;
	glm::vec4 rot;
	unsigned char unknown[32];
	float fov;
};

class camera_update_info
{
public:
	camera_info data;
	float timeSinceUpdated = 0;
	float deltaRot = 0;
	bool getInfoNextFrame = false;
};

typedef __int64(__fastcall* ccamera_set_info)(void* camera, camera_info* info);
typedef __int64(__fastcall* ccamera_set_info2)(void* camera, camera_info* info);