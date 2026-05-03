#include "BattleMapState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include "MainMenuState.h"
#include "BattleState.h"

void BattleMapState::Update(int ch, std::string& lastCommand) {
    UIManager::ClearAllCenterLeftUI();
    UIManager::DisplayStatus();
    UIManager::SetContext(UIPart::Top, 0, "Dungeon Map");
    UIManager::SetContext(UIPart::CenterLeft, 0, "Select a Dungeon Room to enter:");

    // GameManager에서 현재 해금된 진행도를 가져옵니다.
    int unlockedRoom = GameManager::GetInstance().GetProgression();

    // 해금된 층수까지만 입장 메뉴를 띄워줍니다. (최대 4층 = 인덱스 3)
    for (int i = 0; i <= unlockedRoom; ++i) {
        if (i > 3) break;
        UIManager::SetContext(UIPart::CenterLeft, i + 1, std::to_string(i + 1) + ". Enter Floor " + std::to_string(i + 1));
    }
    UIManager::SetContext(UIPart::CenterLeft, 12, "0. Back to Main Menu");

    // 유저 입력 처리
    if (ch == 0) {
        GameManager::GetInstance().SetCurrentState(new MainMenuState());
    }
    // 입력한 번호가 1 이상이고, 해금된 층수 이하일 때만 입장 허용
    else if (ch > 0 && ch <= unlockedRoom + 1 && ch <= 4) {
        GameManager::GetInstance().TrySetProgression(ch - 1); // 선택한 방으로 세팅
        GameManager::GetInstance().SpawnMonster();         // 해당 방의 몬스터 스폰

        UIManager::ClearAllCenterLeftUI();
        GameManager::GetInstance().SetCurrentState(new BattleState());
    }
    else if (ch != -1) {
        UIManager::SetContext(UIPart::CenterLeft, 11, "Locked or Invalid room!");
    }
}
