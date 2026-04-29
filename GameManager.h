#pragma once

class Player;

// 싱글턴
class GameManager {
private:
    GameManager() {} // 숨김
    Player* PLAYER = nullptr; // 전방선언은 포인터만 가능
public:
    ~GameManager();

    // 싱글턴 인스턴스를 가져오는 정적 함수
    inline static GameManager& GetInstance() {
        static GameManager Instance; // static 지역 변수로 인스턴스 생성함
        return Instance;
    }
    GameManager(const GameManager&) = delete; // 복사생성자 금지
    void operator=(const GameManager&) = delete; // 대입 연산 금지

    bool IsGameRunning = false;

    void MakePlayer();

    Player* GetPlayer();

    Player* SetPlayer(Player* newPlayer);
};