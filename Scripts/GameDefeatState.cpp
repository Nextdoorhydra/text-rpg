#include "GameDefeatState.h"
#include "GameManager.h"
#include <iostream>

void GameDefeatState::Update(int ch, std::string& lastCommand) {
    GameManager::GetInstance().IsGameRunning = false;
    // 화면을 지우고 커서를 맨 위(1, 1)로 이동
    std::cout << "\033[2J\033[1;1H";

    for (int i = 0; i < 15; ++i) {
        std::cout << "Game over" << std::endl;
    }
    std::cout << "Enter any key to exit!" << std::endl;

    // 입력을 대기하여 바로 종료되지 않게 함
    std::string temp;
    std::cin >> temp;
}
