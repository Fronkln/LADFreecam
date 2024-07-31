#pragma once
#include "CActionBase.h"
#define OOE_MAX_CAMERAS 64 //estimate
#include <cstdint>

class CCameraBase;

class CActionCameraManager : CActionBase
{

public:
    typedef void(__fastcall* _DoCameraTransition)(void* thisPtr, unsigned int var1, int var2);

    void* cameraArayPtr; //0x01A8
    uint64_t numCameras; //0x01B0
    class CCameraBase* cameraArray[64]; //0x01B8
    char pad_03B8[5888]; //0x03B8
    uint32_t N000075E6; //0x1AB8
    uint32_t activeCameraID; //0x1ABC
    char pad_1AC0[40]; //0x1AC0
    void* currentCameraPtr; //0x1AE8
    char pad_1AF0[32]; //0x1AF0

    CCameraBase* GetCameraByIndex(int index)
    {
        if (index < 0 || index > numCameras - 1)
            return 0;
        else
            return cameraArray[index];
    }
};