#pragma once

#include "../Role.hpp"

class Judge : public Role {
public:
    Judge();
    ~Judge() override;
    
    // Rule of Three
    Judge(const Judge& other);
    Judge& operator=(const Judge& other);
    
    // Can block a bribe
    bool canBlockBribe(const Player& performer) const override;
    
    // When sanctioned, performer pays 1 additional coin
    void onBeingSanctioned(Player& player, Player& performer) override;
};