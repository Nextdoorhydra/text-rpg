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
#include "IGameState.h"
#include "GameStartState.h"
// =================

int main() {
    Controller controller;
    GameManager::GetInstance().MakePlayer();
    GameManager::GetInstance().SpawnMonster();
    GameManager::GetInstance().SetCurrentState(new GameStartState());
    Player* player = GameManager::GetInstance().GetPlayer();
    UIManager::Init();

    // ==============================내배캠 스텝1 요구사항==============================
    std::string userName;
#pragma region GameStart
    std::cout << R"(===========================================
   [ Dungeon Escape Text RPG ]
===========================================
Enter your hero's name: )";

    int stat[4];
    std::cout << "Enter your hero's name: ";
    std::cin >> userName;
    player->Name = userName;

    while (true) {
        std::cout << "Enter HP and MP: ";
        std::cin >> stat[0] >> stat[1];

        std::cout << "Enter Attack and Defense: ";
        std::cin >> stat[2] >> stat[3];

        if (stat[0] > 50 && stat[1] > 50 && stat[2] > 0 && stat[3] > 0) {
            player->SetHp(stat[0]);
            player->MaxHp = stat[0];
            player->Mp = stat[1];
            player->MaxMp = stat[1];
            player->SetAttack(stat[2]);
            player->SetDefense(stat[3]);
            break;
        }

        std::cout << "Invalid input. Please enter positive values greater than 50 for HP, MP." << std::endl;
        std::cout << "Invalid input. Please enter positive values greater than 0 for Attack, Defense." << std::endl;
    }

    // =================================== 도전과제 1 요구사항 =======================================
    int* hpPotionCount = nullptr;
    int* mpPotionCount = nullptr;

    auto HPPotionItem = Item(ItemType::HPPotion, 5);
    auto MPPotionItem = Item(ItemType::MPPotion, 5);

    player->Inven->AddItem(HPPotionItem);
    player->Inven->AddItem(MPPotionItem);

    for (auto& [item, count] : *player->Inven) {
        if (item == HPPotionItem) {
            hpPotionCount = &count;
        }
        else if (item == MPPotionItem) {
            mpPotionCount = &count;
        }
    }

    player->SetPotion(5, hpPotionCount, mpPotionCount);

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

        // 2. UI 렌더링
        UIManager::Render();

        // 3. 유저 입력 처리
        controller.ProcessInput();

        // 4. 입력 파싱 (중복 코드 제거)
        int ch = -1;
        if (!lastCommand.empty()) {
            try {
                ch = std::stoi(lastCommand);
            }
            catch (const std::invalid_argument&) {
                ch = -1;
            }
            catch (const std::out_of_range&) {
                ch = -1;
            }
        }

        // 5. 상태 변화
        IGameState* currentState = GameManager::GetInstance().GetCurrentState();
        if (currentState != nullptr) {
            currentState->Update(ch, lastCommand);
        }

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