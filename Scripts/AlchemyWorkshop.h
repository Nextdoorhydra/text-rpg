#pragma once
#include <string>
#include <vector>
#include <map> // [요구사항] map 추가
#include "PotionRecipe.h"

class Alchemyworkshop {
private:
    const int MAX_STOCK = 3;

    std::vector<PotionRecipe> Recipes;       
    std::map<std::string, int> potionStock_;

public:
    Alchemyworkshop();
    ~Alchemyworkshop();

    std::vector<PotionRecipe> GetRecipes() const;
    void ShowAllRecipes();
    void SearchByName(std::string name);
    void SearchByIngredient(std::string ingredient);

    bool DispensePotion(std::string name);
    bool ReturnPotion(std::string name);
    int GetStock(std::string name);
};