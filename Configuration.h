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
    // 사용 아이템
    HPPotion,
    MPPotion,
    Sword,
    Shield,

    // 재료 아이템
    SlimeJelly,
    GoblinEar,
    OrcHead,
    DragonHorn,
};

enum class Stage {
    MainMenu,
    GameStart,
    Inventory,
    CharacterUpgrade,
    JobSelection,
    Battle,
    AlchemyWorkshop,
    GameClear,
};

// 게임 변수
inline Stage CURRENT_STAGE = Stage::GameStart;
inline Stage PREVIOUS_STAGE = Stage::MainMenu;
inline int PROGRESSION = 0;