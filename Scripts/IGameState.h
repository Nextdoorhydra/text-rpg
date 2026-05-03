#pragma once
#include <string>

class IGameState {
public:
    virtual ~IGameState() = default;

    // 매프레임 실행
    virtual void Update(int ch, std::string& lastCommand) = 0;
};