#pragma once

#ifdef NDEBUG
    #define GAME_RELEASE
#else
    #define GAME_DEBUG
#endif

namespace GameConstants
{
    constexpr int kScreenWidth = 1280;
    constexpr int kScreenHeight = 720;
    constexpr const char* kTitle = "Jam-Giras";
}