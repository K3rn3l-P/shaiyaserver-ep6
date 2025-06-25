#pragma once

void Main();
extern "C" __declspec(dllexport) void DllExport();

//Nuovi setting
#ifndef MAIN_H
#define MAIN_H

extern unsigned u0x41F9ED;

#endif // MAIN_H
//Nuovi setting END

namespace hook
{
    void packet();

    //Nuovi setting
    void camera_limit();
    void cmd();
    void patch();
}

//Nuovi setting
inline int g_showCostumes = false;
inline int g_showPets = false;
inline int g_showWings = false;
inline int g_showEffects = false;
inline int g_showMobEffects = false;
inline float g_cameraLimit = 30.0f;