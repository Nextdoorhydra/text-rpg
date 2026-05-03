#include "MainMenuState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "BattleMapState.h"
#include "JobSelectionState.h"
#include "CharacterUpgradeState.h"
#include "InventoryState.h"
#include "AlchemyWorkshopState.h"
#include "GameDefeatState.h"

void MainMenuState::Update(int ch, std::string& lastCommand) {
    // 1. UI 세팅
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "MainMenu");
    UIManager::SetContext(UIPart::CenterLeft, 0, "1. Enter Dungeon");
    UIManager::SetContext(UIPart::CenterLeft, 1, "2. JobSelection");
    UIManager::SetContext(UIPart::CenterLeft, 2, "3. Character Upgrade");
    UIManager::SetContext(UIPart::CenterLeft, 3, "4. Check Inventory");
    UIManager::SetContext(UIPart::CenterLeft, 4, "5. Potion Shop");
    UIManager::SetContext(UIPart::CenterLeft, 5, "6. QUIT");

    // 2. 상태 객체 교체를 통한 화면 전환
    switch (ch) {
    case 1: GameManager::GetInstance().SetCurrentState(new BattleMapState()); break;
    case 2: GameManager::GetInstance().SetCurrentState(new JobSelectionState()); break;
    case 3: GameManager::GetInstance().SetCurrentState(new CharacterUpgradeState()); break;
    case 4: GameManager::GetInstance().SetCurrentState(new InventoryState()); break;
    case 5: GameManager::GetInstance().SetCurrentState(new AlchemyWorkshopState()); break;
    case 6:
        GameManager::GetInstance().SetCurrentState(new GameDefeatState());
        break;
    }
}
