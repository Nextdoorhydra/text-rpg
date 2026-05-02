#include "Controller.h"
#include "GameManager.h"
#include "UIManager.h"
#include "Configuration.h"
#include <conio.h> // Windows 전용: _kbhit(), _getch()

void Controller::ProcessInput() {
    //UIManager::SetContext(UIPart::CenterLeft, 10, std::string("Last Command: " + lastCommand));

    // 1. 비동기 입력 처리 (Input Blocking 개선)
    if (_kbhit()) {
        char ch = _getch();
        if (ch == '\r') { // Enter 키
            lastCommand = inputBuffer;
            if (lastCommand == "exit") GameManager::GetInstance().IsGameRunning = false;
            inputBuffer.clear();
        }
        else if (ch == '\b') { // Backspace
            if (!inputBuffer.empty()) inputBuffer.pop_back();
        }
        else if (READ_MODE) {
            inputBuffer += ch;
        }
        else if (inputBuffer.length() < 6 && ch >= 48 && ch <= 57) {
            inputBuffer += ch;
        }
    }
}