#pragma once

#include "../Role.hpp"

class General : public Role {
public:
    General();
    ~General() override;
    
    // Rule of Three
    General(const General& other);
    General& operator=(const General& other);
    
    // Block a coup targeting another player or self (costs 5 coins)
    bool canBlockCoup(const Player& performer, const Player& target) const override;
    
    // Regain coins when arrested
    void onBeingArrested(Player& player, Player& performer) override;
};