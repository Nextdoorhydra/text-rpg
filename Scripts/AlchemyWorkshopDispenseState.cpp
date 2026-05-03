#include "AlchemyWorkshopDispenseState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "AlchemyWorkshop.h"
#include "Player.h"
#include "AlchemyWorkshopState.h"

void AlchemyWorkshopDispenseState::Update(int ch, std::string& lastCommand) {
    Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();
    Player* player = GameManager::GetInstance().GetPlayer(); // 인벤토리 접근용

    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop - Dispense");
    UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

    if (ch == 0) {
        READ_MODE = false;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopState());
        return;
    }
    else if (!lastCommand.empty()) {
        std::string targetPotion = lastCommand;
        lastCommand.clear();

        if (workshop->DispensePotion(targetPotion)) {

            if (targetPotion == "HPPotion") player->Inven->AddItem(Item{ ItemType::HPPotion, 5 });
            if (targetPotion == "MPPotion") player->Inven->AddItem(Item{ ItemType::MPPotion, 5 });

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
}
