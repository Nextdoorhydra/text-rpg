#pragma once
#include <string>
#include "IGameState.h"

class InventoryState : public IGameState {
private:
    bool isFromBattle;

public:
    InventoryState(bool fromBattle = false);
    void Update(int ch, std::string& lastCommand) override;
};