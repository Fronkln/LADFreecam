#include "FreecamGame.h"
#include "memory.h"

ccamera_set_info FreecamGame::GetCameraSetFunction()
{
	return (ccamera_set_info)nullptr;
}

void* FreecamGame::GetPadUpdateFunction()
{
	return nullptr;
}

float* FreecamGame::GetSpeedValues() 
{
	return nullptr;
}

void* FreecamGame::GetAuthPauseFunction() 
{
    return nullptr;
}

void FreecamGame::enable_no_input(bool enable)
{
    if (pad_input_func == nullptr)
        return;

    if (enable)
    {
        BYTE patch[] = { 0xC3 };
        mem::Patch(pad_input_func, patch, 1);
    }
    else
    {
        BYTE patch[] = { noInput_originalByte };
        mem::Patch(pad_input_func, patch, 1);
    }
}

void FreecamGame::pause_auth(bool pause)
{

}

void FreecamGame::on_enable()
{

}

void FreecamGame::on_disable() 
{

}

void FreecamGame::init()
{

}

void FreecamGame::update_enabled(float deltaPosX, float deltaPosY, float deltaFocusX, float deltaFocusY, float deltaFov) 
{

}