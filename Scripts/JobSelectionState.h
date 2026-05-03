#pragma once
#include <string>
#include "IGameState.h"

class JobSelectionState : public IGameState {
public:
    void Update(int ch, std::string& lastCommand) override;
};
