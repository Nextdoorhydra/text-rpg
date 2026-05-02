#include "GameManager.h"
#include "Inventory.h"
#include "Player.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Controller.h"
#include <iostream>

#pragma region Animation
void UIManager::DisplayASCIIAnimation() {
    const std::vector<std::vector<std::string>>* targetFrames = nullptr;
    int frameSpeed = 10;

    switch (CURRENT_STAGE) {
    case Stage::BattleMap:
    case Stage::GameStart: {
        static std::vector<std::vector<std::string>> portal = {
            { "                ", "     .---.      ", "   /  . .  \\    ", "   \\  . .  /    ", "     '---'      ", "                " },
            { "                ", "     .---.      ", "   /  / \\  \\    ", "   \\  \\ /  /    ", "     '---'      ", "                " },
            { "                ", "     .---.      ", "   /  - -  \\    ", "   \\  - -  /    ", "     '---'      ", "                " },
            { "      ...       ", "   . :::: .     ", "  . ::::::: .   ", "   . :::: .     ", "      '''       ", "                " },
            { "      ...       ", "   . :::::: .   ", "  . :::::::: .  ", "   . :::::: .   ", "      '''       ", "                " },
            { "      ...       ", "   . :::: .     ", "  . ::::::: .   ", "   . :::: .     ", "      '''       ", "                " }
        };
        targetFrames = &portal;
        frameSpeed = 6;
        break;
    }
    case Stage::MainMenu: {
        static std::vector<std::vector<std::string>> banner = {
            { "  |~~~==~~~|    ", "  |  HERO  |    ", "  |        |    ", "  |  TOWN  |    ", "  |  \\  /  |    ", "  |___\\/___|    " },
            { "   |~~==~~|     ", "   | HERO |     ", "   |      |     ", "   | TOWN |     ", "   | \\  / |     ", "   |__\\/__|     " },
            { "  |~~~~==~~~~|  ", "  |   HERO   |  ", "  |          |  ", "  |   TOWN   |  ", "  |   \\  /   |  ", "  |____\\/____|  " },
            { "   |~~==~~|     ", "   | HERO |     ", "   |      |     ", "   | TOWN |     ", "   | \\  / |     ", "   |__\\/__|     " }
        };
        targetFrames = &banner;
        frameSpeed = 12;
        break;
    }
    case Stage::Inventory: {
        static std::vector<std::vector<std::string>> chest = {
            { "                ", "   __________   ", "  /         /|  ", " +---------+ |  ", " |  [___]  | /  ", " +---------+/   " },
            { "                ", "   .--------.   ", "  /___    _ /|  ", " +----'--'-+ |  ", " |         | /  ", " +---------+/   " },
            { "       * ", "   .--------.   ", "  / _______ /|  ", " +---------+ |  ", " | * * | /  ", " +---------+/   " },
            { "      *** ", "  +---------+   ", "  | _______ |   ", " +---------+ |  ", " |* ITEMS *|/   ", " +---------+/   " }
        };
        targetFrames = &chest;
        frameSpeed = 10;
        break;
    }
    case Stage::JobSelection: {
        static std::vector<std::vector<std::string>> weapons = {
            { "       /\\       ", "       ||       ", "       ||       ", "      ====      ", "       ||       ", "       ()       " },
            { "      (  )      ", "       )(       ", "      (  )      ", "       ||       ", "       ||       ", "       ||       " },
            { "       |\\       ", "       | \\      ", "      -|- >     ", "       | /      ", "       |/       ", "                " },
            { "                ", "                ", "       /\\       ", "       ||       ", "      ====      ", "       ()       " }
        };
        targetFrames = &weapons;
        frameSpeed = 15;
        break;
    }
    case Stage::CharacterUpgrade: {
        static std::vector<std::vector<std::string>> crystal = {
            { "        +       ", "       / \\      ", "      <   >     ", "       \\ /      ", "        v       ", "                " },
            { "       .+.      ", "      /   \\     ", "     <     >    ", "      \\   /     ", "       'v'      ", "                " },
            { "    --  +  --   ", "     / / \\ \\    ", "    < <   > >   ", "     \\ \\ / /    ", "    --  v  --   ", "                " },
            { "       .+.      ", "      /   \\     ", "     <     >    ", "      \\   /     ", "       'v'      ", "                " },
            { "       * * ", "       / \\      ", "      < o >     ", "       \\ /      ", "       * * ", "                " }
        };
        targetFrames = &crystal;
        frameSpeed = 8;
        break;
    }
    case Stage::BattleReward:
    case Stage::Battle: {
        // PROGRESSION에 따른 몬스터 분기
        if (GameManager::GetInstance().GetProgression() == 0) { // Slime
            static std::vector<std::vector<std::string>> slime = {
                { "                ", "      .---.     ", "     / o o \\    ", "    (   \"   )   ", "     '-----'    ", "                " },
                { "                ", "                ", "     .---.      ", "    / - o \\     ", "   (   V   )    ", "                " },
                { "       .-.      ", "      / o o\\    ", "     |  \"   |   ", "      '---'     ", "                ", "                " },
                { "                ", "      .---.     ", "     / o o \\    ", "    (   ~   )   ", "     '-----'    ", "                " }
            };
            targetFrames = &slime;
            frameSpeed = 10;
        }
        else if (GameManager::GetInstance().GetProgression() == 1) { // Goblin
            static std::vector<std::vector<std::string>> goblin = {
                { "  <  ò  ó  >    ", "   \\  vv  /     ", "    |    |      ", "   /|    |\\     ", "    |____|      ", "                " },
                { "  <  ó  ò  >    ", "   \\  vv  /     ", "    |    |      ", "   /|    |\\     ", "    |____|      ", "                " }
            };
            targetFrames = &goblin;
            frameSpeed = 12;
        }
        else if (GameManager::GetInstance().GetProgression() == 2) { // Orc
            static std::vector<std::vector<std::string>> orc = {
                { "   (  ò  ó  )   ", "    \\  --  /    ", "   /|      |\\   ", "  / |______| \\  ", "    |      |    ", "                " },
                { "   (  O  O  )   ", "    \\  --  /    ", "   /|      |\\   ", "  / |______| \\  ", "    |      |    ", "                " }
            };
            targetFrames = &orc;
            frameSpeed = 8;
        }
        else { // Dragon (PROGRESSION >= 3)
            static std::vector<std::vector<std::string>> dragon = {
                { // 1. 숨을 고르며 노려보는 상태
                    "      /\\____/\\  ",
                    "     /  o  o  \\ ",
                    "    (    --    )",
                    "     \\  vvvv  / ",
                    "      \\______/  ",
                    "      /      \\  "
                },
                { // 2. 불꽃이 입가에 맺힘 (연기/불씨)
                    "      /\\____/\\  ",
                    "     /  ò  ò  \\ ",
                    "    (    --    )*",
                    "     \\  WWWW  / *",
                    "      \\______/  ",
                    "      /      \\  "
                },
                { // 3. 화염 방사 (강력한 임팩트)
                    "      /\\____/\\  ",
                    "     /  O  O  \\ @",
                    "    (    --    )@",
                    "     \\  0000  / @",
                    "      \\______/  @",
                    "      /      \\  "
                },
                { // 4. 화염이 소멸하며 입을 다묾
                    "      /\\____/\\  ",
                    "     /  -  -  \\ ",
                    "    (    --    )",
                    "     \\  vvvv  / ",
                    "      \\______/  ",
                    "      /      \\  "
                }
            };
            targetFrames = &dragon;
            frameSpeed = 7; // 화염 방사의 긴박감을 위해 속도를 약간 올림
        }
        break;
    }
    case Stage::AlchemyWorkshopShow:
    case Stage::AlchemyWorkshopSearchByName:
    case Stage::AlchemyWorkshopSearchByIngredient:
    case Stage::AlchemyWorkshopDispense:
    case Stage::AlchemyWorkshopReturn:
    case Stage::AlchemyWorkshop: {
        static std::vector<std::vector<std::string>> cauldron = {
            { "       o        ", "    O           ", "   .-----.      ", "  /_______\\     ", "  \\_______/     ", "   (  )  )      " },
            { "          O     ", "      o         ", "   .-----.      ", "  /_______\\     ", "  \\_______/     ", "   )  (  (      " },
            { "    o           ", "       O        ", "   .-----.      ", "  /_______\\     ", "  \\_______/     ", "  (  (   )      " },
            { "          o     ", "    O           ", "   .-----.      ", "  /_______\\     ", "  \\_______/     ", "   )  )  (      " }
        };
        targetFrames = &cauldron;
        frameSpeed = 8;
        break;
    }
    }

    if (targetFrames == nullptr || targetFrames->empty()) return;

    int startIdx = 7;
    int lastIdx = (int)rightLines.size() - 1;
    int availableHeight = lastIdx - startIdx + 1;

    int currentFrame = (FRAMECOUNT / frameSpeed) % targetFrames->size();
    const auto& art = (*targetFrames)[currentFrame];

    SetContext(UIPart::CenterRight, 6, "");

    for (int i = 0; i < availableHeight; ++i) {
        int targetLine = startIdx + i;
        if (i < (int)art.size()) {
            SetContext(UIPart::CenterRight, targetLine, art[i]);
        }
        else {
            SetContext(UIPart::CenterRight, targetLine, "");
        }
    }
}
#pragma endregion


// ==========================================
// [Internal Helpers]
// ==========================================

static std::string CenterText(const std::string& text, int width) {
    int len = (int)text.length();
    // 한글 등 멀티바이트 처리가 필요한 경우 추가 로직이 필요하나, 
    // 여기서는 기본 너비 절단 및 패딩만 수행
    if (len >= width) return text.substr(0, width);

    int leftPad = (width - len) / 2;
    int rightPad = width - len - leftPad;
    return std::string(leftPad, ' ') + text + std::string(rightPad, ' ');
}

// ==========================================
// [Static Member Definitions]
// ==========================================

std::string UIManager::topStatus = "INITIALIZING...";
std::vector<std::string> UIManager::leftLines;
std::vector<std::string> UIManager::rightLines;

// ==========================================
// [Core Implementation]
// ==========================================

void UIManager::Init() {
    // SCREEN_HEIGHT를 참고하여 본문 높이 계산 (상단 4줄, 하단 3줄 제외)
    int middleHeight = SCREEN_HEIGHT - 7;
    if (middleHeight < 0) middleHeight = 0;

    leftLines.assign(middleHeight, "");
    rightLines.assign(middleHeight, "");
}

void UIManager::DisplayStatus() {
    Player& player = *GameManager::GetInstance().GetPlayer();

    // CenterRight 영역에 6줄에 걸쳐 스테이터스 출력
    SetContext(UIPart::CenterRight, 0, "[ " + player.WhoAmI() + " " + player.Name + " ]");
    SetContext(UIPart::CenterRight, 1, "lv: " + std::to_string(player.GetLevel()) + "(exp: " + std::to_string(player.CurrentEXP) + "/" + "100" + ")");
    SetContext(UIPart::CenterRight, 2, "HP: " + std::to_string(player.GetHp()) + "/" + std::to_string(player.MaxHp));
    SetContext(UIPart::CenterRight, 3, "MP: " + std::to_string(player.Mp) + "/" + std::to_string(player.MaxMp));
    SetContext(UIPart::CenterRight, 4, "ATK: " + std::to_string(player.GetAttack()));
    SetContext(UIPart::CenterRight, 5, "DEF: " + std::to_string(player.GetDefense()));

    // 남은 줄은 공백으로 밀어주기 (이전 잔상 제거)
    for (int i = 6; i < (int)rightLines.size() - 1; ++i) {
        SetContext(UIPart::CenterRight, i, "");
    }
}

void UIManager::DisplayInventory() {
    auto* inven = GameManager::GetInstance().GetPlayer()->Inven;

    int lineIdx = 0;
    int maxLines = (int)leftLines.size(); // SCREEN_HEIGHT - 7

    SetContext(UIPart::CenterLeft, lineIdx++, "======= [ INVENTORY ] =======");

    int itemNumber = 1;

    for (auto const& [item, count] : *inven) {

        if (count <= 0) continue;

        if (lineIdx >= maxLines) break;

        std::string itemInfo = std::to_string(itemNumber++) + ". " + item.Name + " (x" + std::to_string(count) + ")" + " - " + std::to_string(item.Price) + "G";
        SetContext(UIPart::CenterLeft, lineIdx++, itemInfo);
    }

    while (lineIdx < maxLines) {
        SetContext(UIPart::CenterLeft, lineIdx++, "");
    }
}

void UIManager::SetContext(UIPart part, int lineIdx, const std::string& text) {
    switch (part) {
    case UIPart::Top:         topStatus = text; break;
    case UIPart::CenterLeft:
        if (lineIdx >= 0 && lineIdx < (int)leftLines.size()) leftLines[lineIdx] = text;
        break;
    case UIPart::CenterRight:
        if (lineIdx >= 0 && lineIdx < (int)rightLines.size()) rightLines[lineIdx] = text;
        break;
    case UIPart::Bottom:
        // inputBuffer는 전역/외부 관리이므로 필요 시 여기에 복사 로직 추가
        break;
    }
}

void UIManager::Render() {
    // 0. 커서 원점 복귀
    printf("\033[1;1H");

    // 1. 상단 섹션 (3줄)
    for (int x = 0; x < SCREEN_WIDTH; ++x) printf("="); printf("\n");
    char topBuffer[256];
    snprintf(topBuffer, sizeof(topBuffer), "STATUS: %s | Time: %ds",
        topStatus.c_str(), FRAMECOUNT / TARGET_FPS);
    printf("| %s |\n", CenterText(topBuffer, SCREEN_WIDTH - 4).c_str());
    for (int x = 0; x < SCREEN_WIDTH; ++x) printf("="); printf("\n");

    // 2. 본문 섹션 (기둥 유지)
    int infoWidth = 20;
    int mainWidth = SCREEN_WIDTH - infoWidth - 7;
    int middleHeight = SCREEN_HEIGHT - 7; // 상단(3) + 하단(3) + 구분선(1) 제외한 본문 높이

    for (int i = 0; i < middleHeight; ++i) {
        // 내용이 없어도 기둥(|)은 출력함
        std::string lStr = (i < (int)leftLines.size()) ? leftLines[i] : "";
        std::string rStr = (i < (int)rightLines.size()) ? rightLines[i] : "";

        // 본문 기둥 구조: | [Main] | [Info] |
        printf("| %s | %s |\n",
            CenterText(lStr, mainWidth).c_str(),
            CenterText(rStr, infoWidth).c_str());
    }

    // 3. 하단 섹션 (3줄)
    for (int x = 0; x < SCREEN_WIDTH; ++x) printf("="); printf("\n");

    // 입력창 (\033[K로 이전 잔상 지우되 기둥은 다시 그림)
    printf("\033[K");
    printf(" [INPUT] : %s_\n", inputBuffer.c_str());

    for (int x = 0; x < SCREEN_WIDTH; ++x) printf("="); printf("\n");

    FRAMECOUNT++;
    fflush(stdout);
}

void UIManager::DisplayUIPart(UIPart part, int lineIdx, const std::string& text) {
    int targetY = 0, targetX = 0, targetWidth = 0;
    int infoWidth = 20;
    int mainWidth = SCREEN_WIDTH - infoWidth - 7;

    switch (part) {
    case UIPart::Top:
        targetY = 2; targetX = 3; targetWidth = SCREEN_WIDTH - 4;
        break;
    case UIPart::CenterLeft:
        targetY = 4 + lineIdx; // 상단 3줄 이후 첫 번째 본문 줄
        targetX = 3;
        targetWidth = mainWidth;
        break;
    case UIPart::CenterRight:
        targetY = 4 + lineIdx;
        targetX = mainWidth + 6;
        targetWidth = infoWidth;
        break;
    case UIPart::Bottom:
        targetY = SCREEN_HEIGHT - 1;
        targetX = 12;
        targetWidth = SCREEN_WIDTH - 13;
        break;
    }

    // 특정 영역만 덮어쓰기 (기둥 좌표를 건드리지 않음)
    printf("\033[%d;%dH%s", targetY, targetX, CenterText(text, targetWidth).c_str());

    // 커서를 입력창 위치로 복구
    printf("\033[%d;%dH", SCREEN_HEIGHT - 1, 12 + (int)inputBuffer.length());
    fflush(stdout);
}