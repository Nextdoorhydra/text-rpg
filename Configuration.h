#pragma once
#include <chrono>

// 게임 설정
inline const int TARGET_FPS = 60;
inline const auto FRAME_DURATION = std::chrono::milliseconds(1000 / TARGET_FPS);
inline int FRAMECOUNT = 0;
inline const int SCREEN_WIDTH = 60;
inline const int SCREEN_HEIGHT = 20;

// 상수
enum class ItemType {
    HPPotion,
    MPPotion,
    Sword,
    Shield,
};

enum class Stage {
    MainMenu,
    GameStart,
    Inventory,
    CharacterUpgrade,
    JobSelection,
    Battle,
    AlchemyWorkshop,
};

inline Stage CURRENT_STAGE = Stage::GameStart;