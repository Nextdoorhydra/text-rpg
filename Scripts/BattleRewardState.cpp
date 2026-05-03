#include "BattleRewardState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Monster.h"
#include "Player.h"
#include "GameDefeatState.h"
#include "GameClearState.h"
#include "BattleMapState.h"

void BattleRewardState::Update(int ch, std::string& lastCommand) {
    Monster* const& monster = GameManager::GetInstance().GetMonster();
    Player* player = GameManager::GetInstance().GetPlayer();

    int currentFloor = GameManager::GetInstance().GetProgression() + 1;

    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();

    bool isVictory = (player->GetHp() > 0);

    if (isVictory) {
        int rewardExp = 30;

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
            GameManager::GetInstance().SetCurrentState(new GameDefeatState());
        }
        else {
            player->GainExp(30);
            player->Inven->AddItem(monster->dropItem);

            GameManager::GetInstance().AdvanceProgression();

            UIManager::ClearAllCenterLeftUI();

            if (currentFloor >= 4) {
                GameManager::GetInstance().SetCurrentState(new GameClearState());
            }
            else {
                GameManager::GetInstance().SetCurrentState(new BattleMapState());
            }
        }
        break;
    }
}
