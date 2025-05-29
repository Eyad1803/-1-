#pragma once

#include "../Role.hpp"

class Baron : public Role {
public:
    Baron();
    ~Baron() override;
    
    // Rule of Three
    Baron(const Baron& other);
    Baron& operator=(const Baron& other);
    
    // Special invest ability: Pay 3 coins to get 6 coins
    void invest(Player& player, Game& game);
    
    // Get compensation when sanctioned
    void onBeingSanctioned(Player& player, Player& performer) override;
};