#pragma once

#include "../Role.hpp"

class Merchant : public Role {
public:
    Merchant();
    ~Merchant() override;
    
    // Rule of Three
    Merchant(const Merchant& other);
    Merchant& operator=(const Merchant& other);
    
    // Get extra coin at turn start if has at least 3 coins
    void onTurnStart(Player& player, Game& game);
    
    // Special behavior when arrested
    void onBeingArrested(Player& player, Player& performer) override;
};