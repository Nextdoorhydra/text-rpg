#include "AlchemyWorkshop.h"
#include "PotionRecipe.h"
#include "UIManager.h"

Alchemyworkshop::Alchemyworkshop() {
    Recipes = {
        PotionRecipe{"HPPotion", { "EmptyBottle"}},
        PotionRecipe{"MPPotion", { "EmptyBottle"}}
    };

    // 레시피 추가 시 초기화
    for (const auto& r : Recipes) {
        potionStock_[r.Name] = MAX_STOCK;
    }
}

Alchemyworkshop::~Alchemyworkshop() {
}

std::vector<PotionRecipe> Alchemyworkshop::GetRecipes() const {
    return Recipes;
}

[[deprecated("레시피 늘어나는 경우 화면 초과함 확장시 수정할 것")]]
void Alchemyworkshop::ShowAllRecipes() {
    for (int i = 0; i < Recipes.size(); i++) {
        UIManager::SetContext(UIPart::CenterLeft, i, Recipes[i].Name);
    }
}

void Alchemyworkshop::SearchByName(std::string name) {
    for (const auto& r : Recipes) {
        if (r.Name == name) {
            UIManager::SetContext(UIPart::CenterLeft, 0, std::string("Recipe name: ") + r.Name);
            UIManager::SetContext(UIPart::CenterLeft, 1, std::string("===== Ingredient List ====="));
            
            for (int i = 0; i < r.Ingredients.size(); i++) {
                UIManager::SetContext(UIPart::CenterLeft, i + 2, r.Ingredients[i]);
            }
            return;
        }
    }

    UIManager::SetContext(UIPart::CenterLeft, 0, "No recipe found.");
}

void Alchemyworkshop::SearchByIngredient(std::string ingredient) {
    int lineIdx = 0;
    UIManager::SetContext(UIPart::CenterLeft, lineIdx++, "=== Recipes using [" + ingredient + "] ===");

    bool isFound = false;

    // 1. 모든 레시피를 순회
    for (const auto& r : Recipes) {

        bool hasIngredient = false;
        for (const auto& item : r.Ingredients) {
            if (item == ingredient) {
                hasIngredient = true;
                break;
            }
        }

        if (hasIngredient) {
            if (lineIdx <= 12) {
                UIManager::SetContext(UIPart::CenterLeft, lineIdx++, "- " + r.Name);
                isFound = true;
            }
        }
    }

    if (!isFound) {
        UIManager::SetContext(UIPart::CenterLeft, lineIdx, "No recipes found.");
    }
}

bool Alchemyworkshop::DispensePotion(std::string name) {
    if (potionStock_.count(name) > 0 && potionStock_[name] > 0) {
        potionStock_[name]--;
        return true;
    }
    return false; // 재고가 없거나 없는 이름이면 실패
}

bool Alchemyworkshop::ReturnPotion(std::string name) {
    if (potionStock_.count(name) > 0 && potionStock_[name] < MAX_STOCK) {
        potionStock_[name]++;
        return true;
    }
    return false; // MAX_STOCK(3)을 초과하거나 없는 이름이면 실패
}

int Alchemyworkshop::GetStock(std::string name) {
    if (potionStock_.count(name) > 0) {
        return potionStock_[name];
    }
    return -1; // 존재하지 않는 포션
}