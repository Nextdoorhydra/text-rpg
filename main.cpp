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
#include "Thief.h" // 리팩 후 분리
#include "Archer.h" // 리팩 후 분리
#include "Monster.h" // 리팩 후 분리
// =================

static void ClearAllCenterLeftUI() {
    for (int i = 0; i < 13; i++)
        UIManager::SetContext(UIPart::CenterLeft, i, "");
}

int main() {
    Controller controller;
    GameManager::GetInstance().MakePlayer();
    GameManager::GetInstance().SpawnMonster();
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
            player->SetHp(stat[0]);
            player->MaxHp = stat[0];
            player->Mp = stat[1];
            player->MaxMp = stat[1];
            player->SetAttack(stat[2]);
            player->SetDefense(stat[3]);
            break;
        }

        std::cout << "Invalid input. Please enter positive values greater or equal than 50 for HP, MP." << std::endl;
        std::cout << "Invalid input. Please enter positive values greater than 0 for Attack, Defense." << std::endl;
    }

    for (int i = 0; i < 5; ++i) {
        player->Inven->AddItem(Item(ItemType::HPPotion, 5));
        player->Inven->AddItem(Item(ItemType::MPPotion, 5));
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
            PREVIOUS_STAGE = Stage::MainMenu;
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
            PREVIOUS_STAGE = Stage::Battle;
            Monster* const& monster = GameManager::GetInstance().GetMonster();

            // damage 공식
            auto damageFormular = [&](const Character& a, const Character& b) {
                return a.GetAttack() - b.GetDefense() > 0 ? a.GetAttack() - b.GetDefense() : 1;
                };

            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Dungeon: " + std::to_string(PROGRESSION + 1) + " floor");
            UIManager::SetContext(UIPart::CenterLeft, 0, "Enemy: " + monster->Name);
            UIManager::SetContext(UIPart::CenterLeft, 1, "HP: " + std::to_string(monster->GetHp()) + "/" + std::to_string(monster->MaxHp));
            UIManager::SetContext(UIPart::CenterLeft, 2, "ATK: " + std::to_string(monster->GetAttack()));
            UIManager::SetContext(UIPart::CenterLeft, 3, "DEF: " + std::to_string(monster->GetDefense()));
            UIManager::SetContext(UIPart::CenterLeft, 4, "");
            UIManager::SetContext(UIPart::CenterLeft, 5, "");
            UIManager::SetContext(UIPart::CenterLeft, 9, "Choose your strategy");
            UIManager::SetContext(UIPart::CenterLeft, 10, "1. Attack");
            UIManager::SetContext(UIPart::CenterLeft, 11, "2. Inventory");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Retreat");

            switch (ch) {
            case 0: CURRENT_STAGE = Stage::MainMenu; break;
            case 1:
                player->SetHp(player->GetHp() - damageFormular(*monster, *player));
                monster->SetHp(monster->GetHp() - damageFormular(*player, *monster));

                UIManager::SetContext(UIPart::CenterLeft, 4, player->AttackEnemy());
                UIManager::SetContext(UIPart::CenterLeft, 5, monster->AttackEnemy());

                if (monster->GetHp() <= 0) {
                    // 보상
                    player->LevelUp();
                    player->Inven->AddItem(monster->dropItem);

                    // 다음 몬스터 스폰 및 스테이지 진행
                    PROGRESSION++;
                    GameManager::GetInstance().SpawnMonster();

                    ClearAllCenterLeftUI();

                    if(PROGRESSION > 3)
                        CURRENT_STAGE = Stage::GameClear;
                    else
                        CURRENT_STAGE = Stage::MainMenu;
                }

                break;
            case 2: CURRENT_STAGE = Stage::Inventory; break;
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

            if (player->WhoAmI() != "Beginner") {
                ch = ch == 0 ? 0 : 999;
            }

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                UIManager::SetContext(UIPart::CenterLeft, 11, "");
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            case 1:
                GameManager::GetInstance().SetPlayer(new Warrior(player->Name, player->GetHp(), player->Mp, player->GetAttack(), player->GetDefense()));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 2:
                GameManager::GetInstance().SetPlayer(new Mage(player->Name, player->GetHp(), player->Mp, player->GetAttack(), player->GetDefense()));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 3:
                GameManager::GetInstance().SetPlayer(new Thief(player->Name, player->GetHp(), player->Mp, player->GetAttack(), player->GetDefense()));
                player = GameManager::GetInstance().GetPlayer();
                break;
            case 4:
                GameManager::GetInstance().SetPlayer(new Archer(player->Name, player->GetHp(), player->Mp, player->GetAttack(), player->GetDefense()));
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
                switch (PREVIOUS_STAGE) {
                case Stage::MainMenu:
                    CURRENT_STAGE = Stage::MainMenu;
                    break;
                case Stage::Battle:
                    CURRENT_STAGE = Stage::Battle;
                }
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
                break;
            }
            break;
        }
        case Stage::CharacterUpgrade:
        {
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Character Upgrade");
            UIManager::SetContext(UIPart::CenterLeft, 0, "1. HP UP - " + std::to_string(player->Inven->Items[Item(ItemType::HPPotion, 5)]) + " left");
            UIManager::SetContext(UIPart::CenterLeft, 1, "2. MP UP - " + std::to_string(player->Inven->Items[Item(ItemType::MPPotion, 5)]) + " left");
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
                if (player->Inven->TryUseItem(Item(ItemType::HPPotion, 5))) {
                    player->SetHp(player->GetHp() + 20);
                    player->MaxHp += 20;
                    UIManager::SetContext(UIPart::CenterLeft, 11, "HP increased by 20");
                }
                else {
                    UIManager::SetContext(UIPart::CenterLeft, 11, "No HP Potion left");
                }
                break;
            case 2:
                if (player->Inven->TryUseItem(Item(ItemType::MPPotion, 0))) {
                    player->Mp += 20;
                    player->MaxMp += 20;
                    UIManager::SetContext(UIPart::CenterLeft, 11, "MP increased by 20");
                }
                else {
                    UIManager::SetContext(UIPart::CenterLeft, 11, "No MP Potion left");
                }
                break;
            case 3:
                player->SetAttack(player->GetAttack() * 2);
                UIManager::SetContext(UIPart::CenterLeft, 11, "Attack Scaled by 2");
                break;
            case 4:
                player->SetDefense(player->GetDefense() * 2);
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
        case Stage::GameClear: 
        {
            GameManager::GetInstance().IsGameRunning = false;
            // 화면을 지우고 커서를 맨 위(1, 1)로 이동
            std::cout << "\033[2J\033[1;1H";

            for (int i = 0; i < 15; ++i) {
                std::cout << "Congratulations! You Are Dungeon Master!" << std::endl;
            }
                std::cout << "Enter any key to exit!" << std::endl;

            // 입력을 대기하여 바로 종료되지 않게 함
            std::string temp;
            std::cin >> temp;
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