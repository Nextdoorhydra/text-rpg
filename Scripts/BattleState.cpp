#include "BattleState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Monster.h"
#include "Player.h"
#include "BattleMapState.h"
#include "BattleRewardState.h"
#include "InventoryState.h"

void BattleState::Update(int ch, std::string& lastCommand) {
    Monster* const& monster = GameManager::GetInstance().GetMonster();
    Player* player = GameManager::GetInstance().GetPlayer();

    int currentFloor = GameManager::GetInstance().GetProgression() + 1;

    UIManager::ClearAllCenterLeftUI();
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
        UIManager::ClearAllCenterLeftUI();
        GameManager::GetInstance().SetCurrentState(new BattleMapState());
        break;
    case 1:
        UIManager::SetContext(UIPart::CenterLeft, 4, player->AttackEnemy(monster));
        UIManager::SetContext(UIPart::CenterLeft, 5, monster->AttackEnemy(player));

        if (monster->GetHp() <= 0 || player->GetHp() <= 0) {
            GameManager::GetInstance().SetCurrentState(new BattleRewardState());
        }
        break;
    case 2:
        GameManager::GetInstance().SetCurrentState(new InventoryState(true));
        break;
    }
}
