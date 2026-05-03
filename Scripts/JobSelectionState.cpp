#include "JobSelectionState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Player.h"
#include "Warrior.h"
#include "Mage.h"
#include "Thief.h"
#include "Archer.h"
#include "MainMenuState.h"

void JobSelectionState::Update(int ch, std::string& lastCommand) {
    Player* player = GameManager::GetInstance().GetPlayer();

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
        UIManager::SetContext(UIPart::CenterLeft, 11, "");
        UIManager::SetContext(UIPart::CenterLeft, 12, "");
        GameManager::GetInstance().SetCurrentState(new MainMenuState());
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
}
