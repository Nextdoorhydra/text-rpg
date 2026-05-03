#include "InventoryState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Player.h"
#include "MainMenuState.h"
#include "BattleState.h"

InventoryState::InventoryState(bool fromBattle) : isFromBattle(fromBattle) {}

void InventoryState::Update(int ch, std::string& lastCommand) {
    Player* player = GameManager::GetInstance().GetPlayer();
    auto& items = player->Inven;

    UIManager::DisplayInventory();
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "Inventory");
    UIManager::SetContext(UIPart::CenterLeft, 10, "99. Sort Item List by price");
    UIManager::SetContext(UIPart::CenterLeft, 11, "Select item number to use.");
    UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Menu");

    if (ch == 0) {
        UIManager::ClearAllCenterLeftUI();

        if (isFromBattle) {
            GameManager::GetInstance().SetCurrentState(new BattleState());
        }
        else {
            GameManager::GetInstance().SetCurrentState(new MainMenuState());
        }
    }
    else if (ch == 99) {
        items->SortItems([](const auto& a, const auto& b) { return a.first.Price < b.first.Price; });
    }
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
}
