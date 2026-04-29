#pragma once

#include <string>

inline std::string inputBuffer = ""; // 유저가 타이핑 중인 문자열
inline std::string lastCommand = "-1";

class Controller {
    public:
        void ProcessInput();
};