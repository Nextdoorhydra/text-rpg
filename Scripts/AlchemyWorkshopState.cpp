#include "AlchemyWorkshopState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "AlchemyWorkshop.h"
#include "MainMenuState.h"
#include "AlchemyWorkshopShowState.h"
#include "AlchemyWorkshopSearchByNameState.h"
#include "AlchemyWorkshopSearchByIngredientState.h"
#include "AlchemyWorkshopDispenseState.h"
#include "AlchemyWorkshopReturnState.h"

void AlchemyWorkshopState::Update(int ch, std::string& lastCommand) {
    Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

    UIManager::ClearAllCenterLeftUI();
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
        UIManager::ClearAllCenterLeftUI();
        GameManager::GetInstance().SetCurrentState(new MainMenuState());
        break;
    case 1:
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopShowState());
        break;
    case 2:
        READ_MODE = true;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopSearchByNameState());
        break;
    case 3:
        READ_MODE = true;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopSearchByIngredientState());
        break;
    case 4:
        READ_MODE = true;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopDispenseState());
        break;
    case 5:
        READ_MODE = true;
        currentQuery.clear();
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopReturnState());
        break;
    }
}
