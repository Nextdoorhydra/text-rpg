#pragma once
#include <string>
#include <vector>

enum class UIPart { Top, CenterLeft, CenterRight, Bottom };

class UIManager {
public:
    // 초기화 및 프레임 루프용 인터페이스
    static void Init();
    static void Render();
    
    static void DisplayASCIIAnimation();
    static void DisplayStatus();
    static void DisplayInventory();
    static void SetContext(UIPart part, int lineIdx, const std::string& text);

    // [선택사항] 실시간 특정 부분만 덮어쓰기가 필요할 경우를 대비해 유지
    static void DisplayUIPart(UIPart part, int lineIdx, const std::string& text);

private:
    // UI 데이터 상태 저장소
    static std::string topStatus;
    static std::vector<std::string> leftLines;
    static std::vector<std::string> rightLines;
};