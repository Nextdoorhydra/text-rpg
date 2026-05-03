#pragma once
#include <string>
#include "IGameState.h"

class GameStartState : public IGameState {
public:
    void Update(int ch, std::string& lastCommand) override;
};
