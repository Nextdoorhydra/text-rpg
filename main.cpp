#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "Utils.h"
#include "Configuration.h"
#include "UIManager.h"
#include "Controller.h"
#include "GameManager.h"

// =====하드코딩=====
#include "Player.h" // 리팩 후 분리
#include "Warrior.h" // 리팩 후 분리
#include "Mage.h" // 리팩 후 분리
#include "Rogue.h" // 리팩 후 분리
#include "Archer.h" // 리팩 후 분리
// =================

int main() {
    Controller controller;
    GameManager::GetInstance().MakePlayer();
    Player* player = GameManager::GetInstance().GetPlayer();
    UIManager::Init();

    // ==============================내배캠 스텝1 요구사항==============================
    std::string userName;
#pragma region GameStart
    if (CURRENT_STAGE == Stage::GameStart) {
        std::cout << R"(===========================================
   [ Dungeon Escape Text RPG ]
===========================================
Enter your hero's name: )";
    }

    int stat[4];
    std::cout << "Enter your hero's name: ";
    std::cin >> userName;
    player->Name = userName;

    while (true) {
        std::cout << "Enter HP and MP: ";
        std::cin >> stat[0] >> stat[1];

        std::cout << "Enter Attack and Defense: ";
        std::cin >> stat[2] >> stat[3];

        if (stat[0] >= 50 && stat[1] >= 50 && stat[2] > 0 && stat[3] > 0) {
            player->Hp = stat[0];
            player->MaxHp = stat[0];
            player->Mp = stat[1];
            player->MaxMp = stat[1];
            player->Attack = stat[2];
            player->Defense = stat[3];
            break;
        }

        std::cout << "Invalid input. Please enter positive values greater or equal than 50 for HP, MP." << std::endl;
        std::cout << "Invalid input. Please enter positive values greater than 0 for Attack, Defense." << std::endl;
    }

    for (int i = 0; i < 5; ++i) {
        player->Inven->AddItem(ItemType::HPPotion);
        player->Inven->AddItem(ItemType::MPPotion);
    }

    std::cout << "* You received 5 HP Potions and 5 MP Potions." << std::endl;
    std::cout << "Start Game!!!(press any key to continue)" << std::endl;
    std::cin >> userName; // 아무 키나 입력받아서 다음으로 넘어감

    GameManager::GetInstance().IsGameRunning = true;
#pragma endregion
    // ==============================내배캠 스텝1 요구사항==============================

    // 초기 화면 클리어
    std::cout << "\033[2J";
    HideCursor();

#pragma region MAIN_LOOP
    while (GameManager::GetInstance().IsGameRunning) {
        // 1. 프레임 초기화
        UIManager::DisplayASCIIAnimation(); // 재미용 아스키아트
        auto frameStart = std::chrono::steady_clock::now();

        // (주의: 여기서 lastCommand.clear()를 하면 이전 입력을 씹어먹으므로 삭제)

        // 2. UI 렌더링
        UIManager::Render();

        // 3. 유저 입력 처리
        controller.ProcessInput();

        // 4. 입력 파싱 (중복 코드 제거)
        int ch = std::stoi(lastCommand.empty() ? "-1" : lastCommand);

        // 5. 상태 분기 (if -> switch 구조 개선)
        switch (CURRENT_STAGE) {
        case Stage::GameStart:
        case Stage::MainMenu:
        {
            CURRENT_STAGE = Stage::MainMenu;
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "MainMenu");
            UIManager::SetContext(UIPart::CenterLeft, 0, "1. Enter Dungeon");
            UIManager::SetContext(UIPart::CenterLeft, 1, "2. JobSelection");
            UIManager::SetContext(UIPart::CenterLeft, 2, "3. Character Upgrade");
            UIManager::SetContext(UIPart::CenterLeft, 3, "4. Check Inventory");
            UIManager::SetContext(UIPart::CenterLeft, 4, "5. Potion Shop");
            UIManager::SetContext(UIPart::CenterLeft, 5, "6. QUIT");

            switch (ch) {
            case 1: CURRENT_STAGE = Stage::Battle; break;
            case 2: CURRENT_STAGE = Stage::JobSelection; break;
            case 3: CURRENT_STAGE = Stage::CharacterUpgrade; break;
            case 4: CURRENT_STAGE = Stage::Inventory; break;
            case 5: CURRENT_STAGE = Stage::AlchemyWorkshop; break;
            case 6: goto EXIT_GAME;
            }
            break;
        }
        case Stage::Battle:
        {
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Dungeon");

            // 불필요한 코드 간소화 (루프 사용)
            for (int i = 0; i <= 5; ++i) UIManager::SetContext(UIPart::CenterLeft, i, "");

            switch (ch) {
            case 1: CURRENT_STAGE = Stage::Battle; break;
            case 2: CURRENT_STAGE = Stage::JobSelection; break;
            case 3: CURRENT_STAGE = Stage::CharacterUpgrade; break;
            case 4: CURRENT_STAGE = Stage::Inventory; break;
            case 5: CURRENT_STAGE = Stage::AlchemyWorkshop; break;
            case 6: goto EXIT_GAME;
            }
            break;
        }
        case Stage::JobSelection:
        {
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Job Selection");
            UIManager::SetContext(UIPart::CenterLeft, 0, player->Name + ", choose your job!");
            UIManager::SetContext(UIPart::CenterLeft, 1, "1. Warrior");
            UIManager::SetContext(UIPart::CenterLeft, 2, "2. Mage");
            UIManager::SetContext(UIPart::CenterLeft, 3, "3. Rogue");
            UIManager::SetContext(UIPart::CenterLeft, 4, "4. Archer");
            UIManager::SetContext(UIPart::CenterLeft, 5, "");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            if(player->WhoAmI() != "Beginner") {
                ch = ch == 0 ? 0 : 999;
            }

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                UIManager::SetContext(UIPart::CenterLeft, 11, "");
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            case 1:
                GameManager::GetInstance().SetPlayer(new Warrior(player->Name, player->Hp, player->Mp, player->Attack, player->Defense));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 2:
                GameManager::GetInstance().SetPlayer(new Mage(player->Name, player->Hp, player->Mp, player->Attack, player->Defense));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 3:
                GameManager::GetInstance().SetPlayer(new Rogue(player->Name, player->Hp, player->Mp, player->Attack, player->Defense));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 4:
                GameManager::GetInstance().SetPlayer(new Archer(player->Name, player->Hp, player->Mp, player->Attack, player->Defense));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 999:
                UIManager::SetContext(UIPart::CenterLeft, 11, "You have already chosen a job.");
                break;
            }
            break;
        }
        case Stage::Inventory:
        {
            UIManager::DisplayInventory();
            UIManager::SetContext(UIPart::Top, 0, "Inventory");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            }
            break;
        }
        case Stage::CharacterUpgrade:
        {
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Character Upgrade");
            UIManager::SetContext(UIPart::CenterLeft, 0, "1. HP UP - " + std::to_string(player->Inven->Items[ItemType::HPPotion]) + " left");
            UIManager::SetContext(UIPart::CenterLeft, 1, "2. MP UP - " + std::to_string(player->Inven->Items[ItemType::MPPotion]) + " left");
            UIManager::SetContext(UIPart::CenterLeft, 2, "3. Attack x 2");
            UIManager::SetContext(UIPart::CenterLeft, 3, "4. Defense x 2");
            UIManager::SetContext(UIPart::CenterLeft, 4, "");
            UIManager::SetContext(UIPart::CenterLeft, 5, "");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                UIManager::SetContext(UIPart::CenterLeft, 11, "");
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            case 1:
                if (player->Inven->TryUseItem(ItemType::HPPotion)) {
                    player->Hp += 20;
                    player->MaxHp += 20;
                    UIManager::SetContext(UIPart::CenterLeft, 11, "HP increased by 20");
                }
                else {
                    UIManager::SetContext(UIPart::CenterLeft, 11, "No HP Potion left");
                }
                break;
            case 2:
                if (player->Inven->TryUseItem(ItemType::MPPotion)) {
                    player->Mp += 20;
                    player->MaxMp += 20;
                    UIManager::SetContext(UIPart::CenterLeft, 11, "MP increased by 20");
                }
                else {
                    UIManager::SetContext(UIPart::CenterLeft, 11, "No MP Potion left");
                }
                break;
            case 3:
                player->Attack *= 2;
                UIManager::SetContext(UIPart::CenterLeft, 11, "Attack Scaled by 2");
                break;
            case 4:
                player->Defense *= 2;
                UIManager::SetContext(UIPart::CenterLeft, 11, "Defense Scaled by 2");
                break;
            }
            break;
        }
        case Stage::AlchemyWorkshop:
        {
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop");

            for (int i = 0; i <= 5; ++i) UIManager::SetContext(UIPart::CenterLeft, i, "");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            }
            break;
        }
        } // switch(CURRENT_STAGE) 종료

        // 6. 명령어 클
        lastCommand.clear();

        // 7. 프레임 제약
        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsed = frameEnd - frameStart;
        if (elapsed < FRAME_DURATION) {
            std::this_thread::sleep_for(FRAME_DURATION - elapsed);
        }
    }
#pragma endregion


EXIT_GAME: std::cout << "\033[2J\033[1;1 HExit Game" << std::endl;
    return 0;
}