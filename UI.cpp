#include "GameManager.h"
#include "Inventory.h"
#include "Player.h"
#include "UIManager.h"
#include "Configuration.h"
#include "Controller.h"
#include <iostream>

void UIManager::DisplayASCIIAnimation() {
    // 1. 현재 재생할 애니메이션 프레임 포인터와 속도 제어 변수
    const std::vector<std::vector<std::string>>* targetFrames = nullptr;
    int frameSpeed = 10;

    switch (CURRENT_STAGE) {
    case Stage::GameStart: {
        // 소용돌이치는 마법 포탈 (빠른 회전)
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
        // 펄럭이는 길드 깃발 (느긋한 바람)
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
        // 열리는 보물상자 (아이템 확인)
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
        // 무기 변환 (전사:검 -> 법사:지팡이 -> 궁수:활 -> 도적:단검)
        static std::vector<std::vector<std::string>> weapons = {
            { "       /\\       ", "       ||       ", "       ||       ", "      ====      ", "       ||       ", "       ()       " },
            { "      (  )      ", "       )(       ", "      (  )      ", "       ||       ", "       ||       ", "       ||       " },
            { "       |\\       ", "       | \\      ", "      -|- >     ", "       | /      ", "       |/       ", "                " },
            { "                ", "                ", "       /\\       ", "       ||       ", "      ====      ", "       ()       " }
        };
        targetFrames = &weapons;
        frameSpeed = 15; // 직업을 고를 수 있게 무기가 조금 천천히 변하도록 설정
        break;
    }
    case Stage::CharacterUpgrade: {
        // 박동하는 마법 수정 (적당한 속도)
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
    case Stage::Battle: {
        // 탱글탱글한 슬라임 (통통 튀는 속도)
        static std::vector<std::vector<std::string>> slime = {
            { "                ", "      .---.     ", "     / o o \\    ", "    (   \"   )   ", "     '-----'    ", "                " },
            { "                ", "                ", "     .---.      ", "    / - o \\     ", "   (   V   )    ", "                " },
            { "       .-.      ", "      / o o\\    ", "     |  \"   |   ", "      '---'     ", "                ", "                " },
            { "                ", "      .---.     ", "     / o o \\    ", "    (   ~   )   ", "     '-----'    ", "                " }
        };
        targetFrames = &slime;
        frameSpeed = 10;
        break;
    }
    case Stage::AlchemyWorkshop: {
        // 보글보글 끓는 가마솥 (픽하신 아트 적용)
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

    // 예외 처리: 매칭되는 스테이지가 없거나 포인터가 비어있으면 출력 생략
    if (targetFrames == nullptr || targetFrames->empty()) return;

    // 3. 좌표 설정 (마지막 줄까지 꽉 채워 사용)
    int startIdx = 7; // 스탯(0~5) + 여백(6) 다음 줄
    int lastIdx = (int)rightLines.size() - 1;
    int availableHeight = lastIdx - startIdx + 1;

    // 4. 현재 프레임 결정
    int currentFrame = (FRAMECOUNT / frameSpeed) % targetFrames->size();
    const auto& art = (*targetFrames)[currentFrame];

    // 5. 출력 로직
    SetContext(UIPart::CenterRight, 6, ""); // 상단 여백 유지

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
    SetContext(UIPart::CenterRight, 1, "Level: " + std::to_string(player.GetLevel()));
    SetContext(UIPart::CenterRight, 2, "HP: " + std::to_string(player.Hp) + "/" + std::to_string(player.MaxHp));
    SetContext(UIPart::CenterRight, 3, "MP: " + std::to_string(player.Mp) + "/" + std::to_string(player.MaxMp));
    SetContext(UIPart::CenterRight, 4, "ATK: " + std::to_string(player.Attack));
    SetContext(UIPart::CenterRight, 5, "DEF: " + std::to_string(player.Defense));

    // 남은 줄은 공백으로 밀어주기 (이전 잔상 제거)
    for (int i = 6; i < (int)rightLines.size() - 1; ++i) {
        SetContext(UIPart::CenterRight, i, "");
    }
}

static std::string GetItemName(ItemType type) {
    switch (type) {
    case ItemType::HPPotion: return "HP Potion";
    case ItemType::MPPotion: return "MP Potion";
    case ItemType::Sword:    return "Iron Sword";
    case ItemType::Shield:   return "Wood Shield";
    default:                 return "Unknown Item";
    }
}

void UIManager::DisplayInventory() {
    // 맵은 정렬되어 있으므로 일관된 순서로 출력됨
    auto& items = GameManager::GetInstance().GetPlayer()->Inven->Items;

    int lineIdx = 0;
    int maxLines = (int)leftLines.size(); // SCREEN_HEIGHT - 7

    SetContext(UIPart::CenterLeft, lineIdx++, "======= [ INVENTORY ] =======");

    // 맵을 순회하며 아이템 이름과 개수 출력
    int itemNumber = 1;
    for (auto const& [type, count] : items) {
        // 출력 범위를 벗어나면 중단
        if (lineIdx >= maxLines) break;

        std::string itemInfo = std::to_string(itemNumber++) + ". " + GetItemName(type) + " (x" + std::to_string(count) + ")";
        SetContext(UIPart::CenterLeft, lineIdx++, itemInfo);
    }

    // 인벤토리가 비었거나 리스트가 끝난 후 남은 공간 청소
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