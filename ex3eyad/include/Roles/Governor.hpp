#pragma once

#include "../Role.hpp"

class Governor : public Role {
public:
    Governor();
    ~Governor() override;
    
    // Rule of Three
    Governor(const Governor& other);
    Governor& operator=(const Governor& other);
    
    // Override tax to take 3 coins instead of 2
    void onTax(Player& player, Game& game) override;
    
    // Can block tax actions by others
    bool canBlockTax(const Player& performer) const override;
};