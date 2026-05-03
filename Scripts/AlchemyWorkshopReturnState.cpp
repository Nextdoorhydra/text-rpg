#include "AlchemyWorkshopReturnState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "AlchemyWorkshop.h"
#include "Player.h"
#include "AlchemyWorkshopState.h"

void AlchemyWorkshopReturnState::Update(int ch, std::string& lastCommand) {
    Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();
    Player* player = GameManager::GetInstance().GetPlayer();

    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop - Return Bottle");
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
}
