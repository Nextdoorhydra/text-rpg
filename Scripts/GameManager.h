#pragma once

class IGameState;
class Player;
class Monster;
class DungeonRoom;
class Alchemyworkshop;

// 싱글턴
class GameManager {
private:
    GameManager(); // 숨김
    static constexpr int ROOM_SIZE = 4;
    IGameState* CURRENT_STATE = nullptr; // 전방선언은 포인터만 가능
    Player* PLAYER = nullptr;
    Monster* MONSTER = nullptr;
    Alchemyworkshop* ALCHEMY_WORKSHOP = nullptr;
    DungeonRoom* ROOM[ROOM_SIZE]{ nullptr, nullptr, nullptr, nullptr };
    int PROGRESSION = 0;

public:
    ~GameManager();
    GameManager(const GameManager&) = delete; // 복사생성자 금지
    void operator=(const GameManager&) = delete; // 대입 연산 금지

    // 싱글턴 인스턴스를 가져오는 정적 함수
    inline static GameManager& GetInstance() {
        static GameManager Instance; // static 지역 변수로 인스턴스 생성함
        return Instance;
    }

    bool IsGameRunning = false;

    void SetCurrentState(IGameState* newState);
    IGameState* GetCurrentState();

    Alchemyworkshop* GetAlchemyworkshop();

    void MakePlayer();

    Player* GetPlayer();

    Player* SetPlayer(Player* newPlayer);

    Monster* GetMonster();

    Monster* SpawnMonster();

    int GetProgression() const;

    void AdvanceProgression();

    bool TrySetProgression(int k);
};