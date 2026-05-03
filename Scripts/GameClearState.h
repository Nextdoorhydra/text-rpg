#pragma once
#include <string>
#include "IGameState.h"

class GameClearState : public IGameState {
public:
    void Update(int ch, std::string& lastCommand) override;
};
