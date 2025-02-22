// dllmain.cpp : Defines the entry point for the DLL application.

#define GLM_ENABLE_EXPERIMENTAL
#include "pch.h"
#include <iostream>
#include "defines.h"
#include "game.h"
#include "Minhook/MinHook.h"
#include "GLM/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"
#include <cstdint>
#include "memory.h"
#include <map>
#include <thread>

Game currentGame;
Game currentGameName;
FreecamGame* currentGameClass;


bool altControls = false;

bool input_forward;
bool input_back;
bool input_right;
bool input_left;

bool input_move_forward;
bool input_move_back;
bool input_move_right;
bool input_move_left;

bool input_rotate_forward;
bool input_rotate_back;

float moveSpeed = 0.02f;
float lookSpeed = 0.02f;

float deltaPosX = 0;
float deltaPosY = 0;

float deltaFocusX = 0;
float deltaFocusY = 0;

float deltaFov = 0;

int timeStopMode = -1;

bool enabled = false;

float* speed_variables = nullptr;

POINT m_cursorPos;

//Required because of how things seem to be since IW
std::map<__int64, camera_update_info*> m_cameraMap;


bool IsKeyDown(int vk_key)
{
    return GetAsyncKeyState(vk_key) == -32767;
}

bool IsKeyHeld(int vk_key) 
{
   return GetKeyState(vk_key) & 0x8000;
}

vec4f calc_new_focus_point(float cam_x, float cam_y, float cam_z, float speed_x, float speed_y)
{
    float theta = atan2(cam_z, cam_x) + speed_x;

    float phi = sqrt((pow(cam_x, 2) + pow(cam_z, 2)));
    phi = atan2(phi, cam_y) + speed_y;

    float r = sqrt((pow(cam_x, 2) + pow(cam_y, 2) + pow(cam_z, 2)));

    float r_cam_x = r * cos(theta) * sin(phi);
    float r_cam_z = r * sin(theta) * sin(phi);
    float r_cam_y = r * cos(phi);

    vec4f vec = vec4f();
    vec.x = r_cam_x;
    vec.y = r_cam_y;
    vec.z = r_cam_z;

    return vec;
};

//Todo: Fix
vec4f calculate_rotation(vec4f focus, vec4f pos, float rotation) 
{
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::mat4 look = glm::lookAt(glm::vec3(focus.x, focus.y, focus.z), glm::vec3(pos.x, pos.y, pos.z), up);
    
    glm::vec3 direction = glm::normalize(glm::vec3(look[0][2], look[1][2], look[2][2]));
    glm::mat4 m_new = glm::rotateNormalizedAxis(look, rotation, direction);

    vec4f vec = vec4f();
    vec.x = m_new[0][1];
    vec.y = m_new[1][1];
    vec.z = m_new[2][1];

    return vec;
}

float deltaRot = 0;


void input_update() 
{
    deltaFocusX = 0;
    deltaFocusY = 0;
    deltaPosX = 0;
    deltaPosY = 0;

    bool lShift = false;
    bool lAlt = false;
    bool lCtrl = false;

    if (GetKeyState(VK_PRIOR) & 0x8000)
        altControls = true;
    else if (GetKeyState(VK_NEXT) & 0x8000)
        altControls = false;

    if (IsKeyHeld(VK_LMENU))
    {
        lAlt = true;

        if (GetKeyState(VK_UP) & 0x8000)
            deltaFov += 0.01f;

        if (GetKeyState(VK_DOWN) & 0x8000)
            deltaFov -= 0.01f;
    }
    else if (IsKeyHeld(VK_LCONTROL))
    {
        lCtrl = true;

        if (IsKeyHeld(VK_UP))
        {
            lookSpeed += 0.0001f;
            std::cout << "Look speed is " << lookSpeed << std::endl;
        }

        if (IsKeyHeld(VK_DOWN))
        {
            std::cout << "Look speed is " << lookSpeed << std::endl;
            lookSpeed -= 0.0001f;
        }

        if (lookSpeed < 0)
            lookSpeed = 0;
    }
    else if (IsKeyHeld(VK_LSHIFT)) 
    {
        lShift = true;

        if (IsKeyHeld(VK_UP))
        {
            moveSpeed += 0.0001f;
            std::cout << "Speed is " << moveSpeed << std::endl;
        }

        if (IsKeyHeld(VK_DOWN))
        {
            moveSpeed -= 0.0001f;
            std::cout << "Speed is " << moveSpeed << std::endl;
        }

        if (moveSpeed < 0)
            moveSpeed = 0;
    }


    if (!lAlt)
    {
        if (!lCtrl)
        {
            if (input_move_forward)
                deltaPosY += moveSpeed;

            if (input_move_back)
                deltaPosY -= moveSpeed;

            if (input_move_left)
                deltaPosX += moveSpeed;

            if (input_move_right)
                deltaPosX -= moveSpeed;
        }

        if (!lShift && !lCtrl)
        {
            if (input_forward)
                deltaFocusY -= lookSpeed;

            if (input_back)
                deltaFocusY += lookSpeed;

            if (input_left)
                deltaFocusX -= lookSpeed;

            if (input_right)
                deltaFocusX += lookSpeed;
        }

        if (input_rotate_forward)
            deltaRot += lookSpeed;

        if (input_rotate_back)
            deltaRot -= lookSpeed;
    }
    
}

//for non-DE games
void update_thread() 
{
    while (true)
    {
        if (enabled)
        {
            input_update();
            currentGameClass->update_enabled(deltaPosX, deltaPosY, deltaFocusX, deltaFocusY, deltaFov);
        }

        Sleep(1);
    }
}

void update_common(void* camera_entity, camera_info* info)
{
    camera_info* ourInfoPtr = &m_cameraMap[(__int64)camera_entity]->data;
    camera_info _currentInfo = *ourInfoPtr;

    float r_cam_x = _currentInfo.focus.x - _currentInfo.pos.x;
    float r_cam_y = _currentInfo.focus.y - _currentInfo.pos.y;
    float r_cam_z = _currentInfo.focus.z - _currentInfo.pos.z;

    vec4f r;
    r.x = r_cam_x;
    r.y = r_cam_y;
    r.z = r_cam_z;

    vec4f focusOut = calc_new_focus_point(r_cam_x, r_cam_y, r_cam_z, deltaFocusX, deltaFocusY);

    _currentInfo.pos.x += r_cam_x * deltaPosY + deltaPosX * r_cam_z;
    _currentInfo.pos.y += r_cam_y * deltaPosY;
    _currentInfo.pos.z += r_cam_z * deltaPosY - deltaPosX * r_cam_x;

    vec4f newFocus;
    newFocus.x = _currentInfo.pos.x + focusOut.x;
    newFocus.y = _currentInfo.pos.y + focusOut.y;
    newFocus.z = _currentInfo.pos.z + focusOut.z;

    _currentInfo.focus = newFocus;

    _currentInfo.rot = calculate_rotation(newFocus, _currentInfo.pos, deltaRot);

    info->pos = _currentInfo.pos;
    info->focus = _currentInfo.focus;
    info->rot = _currentInfo.rot;
    info->fov = _currentInfo.fov + deltaFov;

    *ourInfoPtr = _currentInfo;
}

void enable_no_input(bool enable)
{
    currentGameClass->enable_no_input(enable);
}

//Main camera update code.
ccamera_set_info _update_camera_trampoline = nullptr;
__int64 update_camera(void* camera_entity, camera_info* info)
{
    if (!enabled)
    {
        enable_no_input(false);
        return _update_camera_trampoline(camera_entity, info);
    }

    __int64 cami64 = (__int64)camera_entity;
    
    //New camera update
    if (!m_cameraMap.count(cami64))
    {
        camera_update_info* updateInf = new camera_update_info();
        updateInf->getInfoNextFrame = true;
        updateInf->data.pos = info->pos;
        updateInf->data.focus = info->focus;
        updateInf->data = *info;
        updateInf->deltaRot = 0;

        m_cameraMap.insert({ cami64, updateInf });
        
        std::cout << "new cam" << std::endl;
        return _update_camera_trampoline(camera_entity, info);
    }

    camera_update_info* updateInf = m_cameraMap[cami64];

    //Necessary for proper initialization
    if (updateInf->getInfoNextFrame)
    {
        updateInf->data = *info;
        updateInf->getInfoNextFrame = false;

        enable_no_input(true);
    }

    input_update(); //input update
    update_common(camera_entity, info); //general purpose DE movement update
    currentGameClass->update_enabled(deltaPosX, deltaPosY, deltaFocusX, deltaFocusY, deltaFov);
    
    return _update_camera_trampoline(camera_entity, info);
}


DWORD WINAPI AppThread(HMODULE hModule)
{
#if _DEBUG
    std::cout << "Starting in 2 seconds...\n";
    Sleep(2000);
#endif

    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    // Get the name of the current game
    wchar_t exePath[MAX_PATH + 1];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    wstring wstr(exePath);
    string currentGameName = basenameBackslashNoExt(string(wstr.begin(), wstr.end()));

    currentGame = getGame(currentGameName);
    currentGameName = getGameName(currentGame);
    currentGameClass = get_game_class(currentGame);
    currentGameClass->isGog = GetModuleHandle(L"galaxy64.dll") != nullptr;

    std::cout << "\nHello from Freecam world! " << "Game is: " << currentGameName.c_str() << std::endl;

    //Necessary hooking and patching
    std::cout << "MinHook initialize result: " << (MH_Initialize() == MH_OK ? "OKAY!" : "FAIL!") << std::endl;

    ccamera_set_info camera_func = currentGameClass->GetCameraSetFunction();

    if (camera_func != nullptr)
        std::cout << "Camera function found! " << std::hex << camera_func << std::endl;
    else
    {
        std::cout << "Could not find camera function!" << std::endl;
    }

    std::cout << "Camera create hook result: " << (MH_CreateHook(camera_func, update_camera, (LPVOID*)&_update_camera_trampoline) == MH_OK ? "OKAY!" : "FAIL!") << std::endl;

    currentGameClass->pad_input_func = (BYTE*)currentGameClass->GetPadUpdateFunction();

    if (currentGameClass->pad_input_func != nullptr)
        std::cout << "Input block function found! " << std::hex << (void*)currentGameClass->pad_input_func << std::endl;
    else
        std::cout << "Could not find input block function!" << std::endl;

    if(currentGameClass->pad_input_func != nullptr)
        currentGameClass->noInput_originalByte = *currentGameClass->pad_input_func;

    currentGameClass->auth_pause_func = currentGameClass->GetAuthPauseFunction();

    if (currentGameClass->auth_pause_func != nullptr)
        std::cout << "Auth pause function found! " << std::hex << currentGameClass->auth_pause_func << std::endl;
    else
        std::cout << "Could not find auth pause function!" << std::endl;

    speed_variables = currentGameClass->GetSpeedValues();

    if (speed_variables != nullptr)
        std::cout << "Speed control values found!" << std::hex << speed_variables << std::endl;
    else
        std::cout << "Could not find speed control values!" << std::endl;

    currentGameClass->init();

    if (currentGame <= Game::Yakuza5)
    {
        std::thread updateThread(update_thread);
        updateThread.detach();
    }

    std::cout << "**********************\n";
    std::cout << "Yakuza Freecam Active!\n";
    std::cout << "**********************\n";

    std::cout 
        << "Alt+M - Enable/Disable\n"
        << "Alt+T - Change between Everything/NPC/Player Gameplay Time Stop\n\n"
        << "Page Up - Enable mouselook mode\nPage Down - Disable mouselook mode\n\n"
        << "LShift+Up/Down Arrow - Adjust Movement Speed\n"
        << "LCtrl+Up/Down Arrow - Adjust Look Speed\n"
        << "LAlt+Up/Down Arrow - Adjust FOV\n\n"
        << "WASD - Move Camera\nArrow Keys - Rotate Camera\n";


    while (true)
    {
        //Gameplay time control
        if (timeStopMode > -1 && speed_variables != nullptr)
        {
            if (timeStopMode == 0)
            {
                speed_variables[0] = 0.001f;
                speed_variables[1] = 0.001f;
                speed_variables[2] = 0.001f;
            }
            else if (timeStopMode == 1)
            {
                speed_variables[0] = 0.001f;
                speed_variables[2] = 0.001f;
            }
            else if (timeStopMode == 2)
            {
                speed_variables[1] = 0.001f;
            }
        }

        if (enabled)
        {
            //Movement inputs
            if (!altControls)
            {
                input_move_forward = GetKeyState(0x57) & 0x8000;
                input_move_back = GetKeyState(0x53) & 0x8000;
                input_move_left = GetKeyState(0x41) & 0x8000;
                input_move_right = GetKeyState(0x44) & 0x8000;

                input_forward = GetKeyState(VK_UP) & 0x8000;
                input_left = GetKeyState(VK_LEFT) & 0x8000;
                input_right = GetKeyState(VK_RIGHT) & 0x8000;
                input_back = GetKeyState(VK_DOWN) & 0x8000;
            }
            else
            {
                input_move_forward = GetKeyState(0x57) & 0x8000;
                input_move_back = GetKeyState(0x53) & 0x8000;
                input_move_left = GetKeyState(0x41) & 0x8000;
                input_move_right = GetKeyState(0x44) & 0x8000;
            }
            input_rotate_forward = GetKeyState(0x51) & 0x8000;
            input_rotate_back = GetKeyState(0x45) & 0x8000;              
        }

        //Other inputs
        if (GetKeyState(VK_LMENU) & 0x8000)
        {
            if (IsKeyDown(0x4D))
            {
                if (!enabled)
                {
                    enabled = true;
                    MH_EnableHook(MH_ALL_HOOKS);

                    currentGameClass->pause_auth(true);
                    currentGameClass->on_enable();

                    std::cout << "Enabled freecam.\n";
                }
                else
                {
                    if (enabled)
                    {
                        enabled = false;
                        MH_DisableHook(MH_ALL_HOOKS);
                        currentGameClass->pause_auth(false);
                        enable_no_input(false);

                        deltaFov = 0;
                        deltaRot = 0;

                        for (auto const& x : m_cameraMap)
                            delete x.second;

                        m_cameraMap.clear();

                        currentGameClass->on_disable();

                        std::cout << "Disabled freecam.\n";
                    }
                }
            }

            if (IsKeyDown(0x54))
            {
                timeStopMode++;

                if (timeStopMode > 2)
                    timeStopMode = -1;

                std::cout << "Timestop mode: ";
                             
                if (timeStopMode == 0)
                    std::cout << "Everything";
                else if (timeStopMode == 1)
                    std::cout << "NPC";
                else if (timeStopMode == 2)
                    std::cout << "Player";
                else if (timeStopMode == -1)
                    std::cout << "Disabled";

                std::cout << std::endl;
            }
        }
        
        if (enabled)
        {
            if (altControls)
            {
                input_forward = false;
                input_back = false;
                input_left = false;
                input_right = false;
            }
        }

        if (enabled)
        {

            //Worst mouse look code youhave ever seen
            if (altControls)
            {
                RECT wRect;
                if (GetWindowRect(GetDesktopWindow(), &wRect))
                {
                    POINT p;

                    if (GetCursorPos(&p))
                    {
                        if (p.y > m_cursorPos.y)
                            input_back = true;
                        else if (p.y < m_cursorPos.y)
                            input_forward = true;

                        if (p.x > m_cursorPos.x)
                            input_right = true;
                        else if (p.x < m_cursorPos.x)
                            input_left = true;
                    }

                    m_cursorPos.x = wRect.right / 2;
                    m_cursorPos.y = wRect.bottom / 2;

                    SetCursorPos(wRect.right / 2, wRect.bottom / 2);
                }
            }
        }
        Sleep(2);
    }

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)AppThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}