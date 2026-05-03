#include "CharacterUpgradeState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Player.h"
#include "MainMenuState.h"

void CharacterUpgradeState::Update(int ch, std::string& lastCommand) {
    Player* player = GameManager::GetInstance().GetPlayer();

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
        GameManager::GetInstance().SetCurrentState(new MainMenuState());
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
}
