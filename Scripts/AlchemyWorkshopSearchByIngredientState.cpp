#include "AlchemyWorkshopSearchByIngredientState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "AlchemyWorkshop.h"
#include "AlchemyWorkshopState.h"

void AlchemyWorkshopSearchByIngredientState::Update(int ch, std::string& lastCommand) {
    Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop Search by ingredient");
    UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

    if (ch == 0) {
        READ_MODE = false;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopState());
        return;
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
}
