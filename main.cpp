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
#include "Alchemyworkshop.h" // 리팩 후 분리
// =================

static void ClearAllCenterLeftUI() {
    for (int i = 0; i < 13; i++)
        UIManager::SetContext(UIPart::CenterLeft, i, "");
}

// 검색모드 사용
static std::string currentQuery = "";

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
            case 1: CURRENT_STAGE = Stage::BattleMap; break; // [수정됨] 바로 전투로 안 가고 맵으로 이동
            case 2: CURRENT_STAGE = Stage::JobSelection; break;
            case 3: CURRENT_STAGE = Stage::CharacterUpgrade; break;
            case 4: CURRENT_STAGE = Stage::Inventory; break;
            case 5: CURRENT_STAGE = Stage::AlchemyWorkshop; break;
            case 6: goto EXIT_GAME;
            }
            break;
        }

        case Stage::BattleMap: // [신규 구현] 원하는 방 선택
        {
            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Dungeon Map");
            UIManager::SetContext(UIPart::CenterLeft, 0, "Select a Dungeon Room to enter:");

            // GameManager에서 현재 해금된 진행도를 가져옵니다.
            int unlockedRoom = GameManager::GetInstance().GetProgression();

            // 해금된 층수까지만 입장 메뉴를 띄워줍니다. (최대 4층 = 인덱스 3)
            for (int i = 0; i <= unlockedRoom; ++i) {
                if (i > 3) break;
                UIManager::SetContext(UIPart::CenterLeft, i + 1, std::to_string(i + 1) + ". Enter Floor " + std::to_string(i + 1));
            }
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            // 유저 입력 처리
            if (ch == 0) {
                CURRENT_STAGE = Stage::MainMenu;
            }
            // 입력한 번호가 1 이상이고, 해금된 층수 이하일 때만 입장 허용
            else if (ch > 0 && ch <= unlockedRoom + 1 && ch <= 4) {
                GameManager::GetInstance().TrySetProgression(ch - 1); // 선택한 방으로 세팅
                GameManager::GetInstance().SpawnMonster();         // 해당 방의 몬스터 스폰

                ClearAllCenterLeftUI();
                CURRENT_STAGE = Stage::Battle;
            }
            else if (ch != -1) {
                UIManager::SetContext(UIPart::CenterLeft, 11, "Locked or Invalid room!");
            }
            break;
        }

        case Stage::Battle:
        {
            PREVIOUS_STAGE = Stage::Battle;
            Monster* const& monster = GameManager::GetInstance().GetMonster();

            // [수정됨] 전역 변수 대신 GameManager의 getter 사용
            int currentFloor = GameManager::GetInstance().GetProgression() + 1;

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Dungeon: " + std::to_string(currentFloor) + " floor");
            UIManager::SetContext(UIPart::CenterLeft, 0, "Enemy: " + monster->Name);
            UIManager::SetContext(UIPart::CenterLeft, 1, "HP: " + std::to_string(monster->GetHp()) + "/" + std::to_string(monster->MaxHp));
            UIManager::SetContext(UIPart::CenterLeft, 2, "ATK: " + std::to_string(monster->GetAttack()));
            UIManager::SetContext(UIPart::CenterLeft, 3, "DEF: " + std::to_string(monster->GetDefense()));
            UIManager::SetContext(UIPart::CenterLeft, 9, "Choose your strategy");
            UIManager::SetContext(UIPart::CenterLeft, 10, "1. Attack");
            UIManager::SetContext(UIPart::CenterLeft, 11, "2. Inventory");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Retreat");

            switch (ch) {
            case 0:
                ClearAllCenterLeftUI();
                CURRENT_STAGE = Stage::BattleMap; // [수정됨] 도망치면 맵으로 복귀
                break;
            case 1:
                UIManager::SetContext(UIPart::CenterLeft, 4, player->AttackEnemy(monster));
                UIManager::SetContext(UIPart::CenterLeft, 5, monster->AttackEnemy(player));

                if (monster->GetHp() <= 0 || player->GetHp() <= 0) {
                    CURRENT_STAGE = Stage::BattleReward;
                }
                break;
            case 2: CURRENT_STAGE = Stage::Inventory; break;
            }
            break;
        }

        case Stage::BattleReward:
        {
            PREVIOUS_STAGE = Stage::Battle;
            Monster* const& monster = GameManager::GetInstance().GetMonster();

            // [수정됨] PROGRESSION 에러 방지
            int currentFloor = GameManager::GetInstance().GetProgression() + 1;

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();

            bool isVictory = (player->GetHp() > 0);

            if (isVictory) {
                int rewardExp = 30; // 임시 경험치

                UIManager::SetContext(UIPart::Top, 0, "Dungeon: " + std::to_string(currentFloor) + " floor - CLEAR!");
                UIManager::SetContext(UIPart::CenterLeft, 0, "  < VICTORY >  ");
                UIManager::SetContext(UIPart::CenterLeft, 2, "- Obtained Item: " + monster->dropItem.Name);
                UIManager::SetContext(UIPart::CenterLeft, 3, "- Gained EXP: " + std::to_string(rewardExp));
            }
            else {
                UIManager::SetContext(UIPart::Top, 0, "Dungeon: " + std::to_string(currentFloor) + " floor - FAILED");
                UIManager::SetContext(UIPart::CenterLeft, 0, "  < DEFEAT >  ");
                UIManager::SetContext(UIPart::CenterLeft, 2, "You have fallen...");
            }

            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Next");

            switch (ch) {
            case 0:
                if (!isVictory) {
                    CURRENT_STAGE = Stage::GameDefeat;
                }
                else {
                    // 승리 보상 지급
                    player->GainExp(30);
                    player->Inven->AddItem(monster->dropItem);

                    // 최고 기록 갱신 (만약 지금 깬 방이 내 최고 도달 층수라면 다음 방 해금)
                    GameManager::GetInstance().AdvanceProgression();

                    ClearAllCenterLeftUI();

                    // 마지막 4층을 깼다면 게임 클리어, 아니면 배틀 맵으로 복귀시켜 다시 선택하게 함
                    if (currentFloor >= 4) {
                        CURRENT_STAGE = Stage::GameClear;
                    }
                    else {
                        CURRENT_STAGE = Stage::BattleMap;
                    }
                }
                break;
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
            Player* player = GameManager::GetInstance().GetPlayer();
            auto& items = player->Inven; // items는 Inventory<Item>* 타입

            UIManager::DisplayInventory();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Inventory");
            UIManager::SetContext(UIPart::CenterLeft, 10, "99. Sort Item List by price");
            UIManager::SetContext(UIPart::CenterLeft, 11, "Select item number to use.");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Menu");

            // 유저 입력 처리
            if (ch == 0) {
                switch (PREVIOUS_STAGE) {
                case Stage::MainMenu: CURRENT_STAGE = Stage::MainMenu; break;
                case Stage::Battle: CURRENT_STAGE = Stage::Battle; break;
                }
                UIManager::SetContext(UIPart::CenterLeft, 11, "");
                UIManager::SetContext(UIPart::CenterLeft, 12, "");
            }
            else if (ch == 99) {
                items->SortItems([](const auto& a, const auto& b) { return a.first.Price < b.first.Price; });
            }
            // 입력 번호가 1 이상이고, 현재 인벤토리 아이템 종류 개수 이하일 때
            else if (ch > 0 && ch <= items->GetSize()) {

                Item selectedItem = (*items)[ch - 1].first;

                if (player->Inven->TryUseItem(selectedItem)) {
                    switch (selectedItem.Type) {
                    case ItemType::HPPotion:
                        player->SetHp(player->GetHp() + 50);
                        UIManager::SetContext(UIPart::CenterLeft, 11, "Used HP Potion! (HP +50)");
                        player->Inven->AddItem(Item{ ItemType::EmptyBottle, 0 });
                        break;

                    case ItemType::MPPotion:
                        player->Mp += 50;
                        if (player->Mp > player->MaxMp) player->Mp = player->MaxMp;
                        UIManager::SetContext(UIPart::CenterLeft, 11, "Used MP Potion! (MP +50)");
                        player->Inven->AddItem(Item{ ItemType::EmptyBottle, 0 });
                        break;

                    default:
                        UIManager::SetContext(UIPart::CenterLeft, 11, "Used an item with no specific effect.");
                        break;
                    }
                }
                else {
                    UIManager::SetContext(UIPart::CenterLeft, 11, "Cannot use this item.");
                }
            }
            else if (ch != -1) {
                UIManager::SetContext(UIPart::CenterLeft, 11, "Invalid item number!");
            }

            break;
        }
        case Stage::CharacterUpgrade:
        {
            auto hpPotion = Item(ItemType::HPPotion, 5);
            auto mpPotion = Item(ItemType::MPPotion, 5);

            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Character Upgrade");
            UIManager::SetContext(UIPart::CenterLeft, 0, "1. HP UP - " + std::to_string(player->Inven->GetItemCount(hpPotion)) + " left");
            UIManager::SetContext(UIPart::CenterLeft, 1, "2. MP UP - " + std::to_string(player->Inven->GetItemCount(mpPotion)) + " left");
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
                    player->Inven->AddItem(Item{ ItemType::EmptyBottle, 0 });
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
                    player->Inven->AddItem(Item{ ItemType::EmptyBottle, 0 });
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
    #pragma region Alchemy
        case Stage::AlchemyWorkshop:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop");
            UIManager::SetContext(UIPart::CenterLeft, 0, "=== Potion Shop ===");
            UIManager::SetContext(UIPart::CenterLeft, 1, "1. Show all recipes");
            UIManager::SetContext(UIPart::CenterLeft, 2, "2. Search by potion name");
            UIManager::SetContext(UIPart::CenterLeft, 3, "3. Search by ingredient");
            UIManager::SetContext(UIPart::CenterLeft, 4, "4. Dispense Potion");
            UIManager::SetContext(UIPart::CenterLeft, 5, "5. Return Empty Bottle");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

            switch (ch) {
            case 0:
                CURRENT_STAGE = Stage::MainMenu;
                ClearAllCenterLeftUI();
                break;
            case 1:
                CURRENT_STAGE = Stage::AlchemyWorkshopShow;
                break;
            case 2:
                READ_MODE = true;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshopSearchByName;
                break;
            case 3:
                READ_MODE = true;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshopSearchByIngredient;
                break;
            case 4:
                READ_MODE = true;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshopDispense;
                break;
            case 5:
                READ_MODE = true;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshopReturn;
                break;
            }
            break;
        }
        case Stage::AlchemyWorkshopShow:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            workshop->ShowAllRecipes();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop Show");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");
                        
            switch (ch) {
            case 0:
                READ_MODE = false;
                CURRENT_STAGE = Stage::AlchemyWorkshop;
                ClearAllCenterLeftUI();
                break;
            }
            break;
        }
        case Stage::AlchemyWorkshopSearchByName:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop Search by name");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

            if (ch == 0) {
                READ_MODE = false;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshop;
                break;
            }
            else if (!lastCommand.empty()) {
                currentQuery = lastCommand; // 입력된 검색어를 기억함
            }

            if (currentQuery.empty()) {
                UIManager::SetContext(UIPart::CenterLeft, 0, "Read Mode is available");
                UIManager::SetContext(UIPart::CenterLeft, 1, "Type Recipe name you find:");
            }
            else {
                workshop->SearchByName(currentQuery);
            }
            break;
        }
        case Stage::AlchemyWorkshopSearchByIngredient:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop Search by ingredient");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

            if (ch == 0) {
                READ_MODE = false;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshop;
                break;
            }
            else if (!lastCommand.empty()) {
                currentQuery = lastCommand;
            }

            if (currentQuery.empty()) {
                UIManager::SetContext(UIPart::CenterLeft, 0, "Read Mode is available");
                UIManager::SetContext(UIPart::CenterLeft, 1, "Type Ingredient name you find:");
            }
            else {
                workshop->SearchByIngredient(currentQuery);
            }
            break;
        }
        case Stage::AlchemyWorkshopDispense:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();
            Player* player = GameManager::GetInstance().GetPlayer(); // 인벤토리 접근용

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop - Dispense");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

            if (ch == 0) {
                READ_MODE = false;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshop;
                break;
            }
            else if (!lastCommand.empty()) {
                std::string targetPotion = lastCommand;
                lastCommand.clear();

                if (workshop->DispensePotion(targetPotion)) {

                     if(targetPotion == "HPPotion") player->Inven->AddItem(Item{ ItemType::HPPotion, 5 });
                     if(targetPotion == "MPPotion") player->Inven->AddItem(Item{ ItemType::MPPotion, 5 });

                    currentQuery = "Dispense " + targetPotion + " (stock: " + std::to_string(workshop->GetStock(targetPotion)) + ")";
                }
                else {
                    currentQuery = "Dispense failed";
                }
            }
                        
            UIManager::SetContext(UIPart::CenterLeft, 0, "Type Potion Name to Dispense:");
            UIManager::SetContext(UIPart::CenterLeft, 1, currentQuery);

            // 전체 재고 현황을 보여줌
            int line = 3;
            UIManager::SetContext(UIPart::CenterLeft, line++, "[ Current Stock ]");
            for (const auto& r : workshop->GetRecipes()) { 
                UIManager::SetContext(UIPart::CenterLeft, line++, "- " + r.Name + " : " + std::to_string(workshop->GetStock(r.Name)));
            }
            break;
        }
        case Stage::AlchemyWorkshopReturn:
        {
            Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();
            Player* player = GameManager::GetInstance().GetPlayer();

            ClearAllCenterLeftUI();
            UIManager::DisplayStatus();
            UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop - Return Bottle");
            UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

            if (ch == 0) {
                READ_MODE = false;
                currentQuery.clear();
                CURRENT_STAGE = Stage::AlchemyWorkshop;
                break;
            }
            else if (!lastCommand.empty()) {
                std::string targetPotion = lastCommand;
                lastCommand.clear();

                if (player->Inven->TryUseItem(Item{ ItemType::EmptyBottle, 0 }) && workshop->ReturnPotion(targetPotion)) {
                    currentQuery = "Return empty bottle for " + targetPotion + " - " + std::to_string(workshop->GetStock(targetPotion)) + " EA";
                }
                else {
                    currentQuery = "Return failed";
                }
            }

            UIManager::SetContext(UIPart::CenterLeft, 0, "Type Potion Name to Return Bottle:");
            UIManager::SetContext(UIPart::CenterLeft, 1, currentQuery);

            int line = 3;
            UIManager::SetContext(UIPart::CenterLeft, line++, "[ Current Stock ]");
            for (const auto& r : workshop->GetRecipes()) {
                UIManager::SetContext(UIPart::CenterLeft, line++, "- " + r.Name + " : " + std::to_string(workshop->GetStock(r.Name)));
            }
            break;
        }
    #pragma endregion
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

            break;
        }
        case Stage::GameDefeat: 
        {
            GameManager::GetInstance().IsGameRunning = false;
            // 화면을 지우고 커서를 맨 위(1, 1)로 이동
            std::cout << "\033[2J\033[1;1H";

            for (int i = 0; i < 15; ++i) {
                std::cout << "Game over" << std::endl;
            }
                std::cout << "Enter any key to exit!" << std::endl;

            // 입력을 대기하여 바로 종료되지 않게 함
            std::string temp;
            std::cin >> temp;

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