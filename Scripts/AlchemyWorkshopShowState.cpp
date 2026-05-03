#include "AlchemyWorkshopShowState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "AlchemyWorkshop.h"
#include "AlchemyWorkshopState.h"

void AlchemyWorkshopShowState::Update(int ch, std::string& lastCommand) {
    Alchemyworkshop* workshop = GameManager::GetInstance().GetAlchemyworkshop();

    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();
    workshop->ShowAllRecipes();
    UIManager::SetContext(UIPart::Top, 0, "Alchemy Workshop Show");
    UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Potion Shop");

    switch (ch) {
    case 0:
        READ_MODE = false;
        GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopState());
        UIManager::ClearAllCenterLeftUI();
        break;
    }
}
