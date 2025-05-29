#pragma once

#include "../Role.hpp"

class Spy : public Role {
private:
    bool hasSpiedOnPlayer; // Track if the spy has already used their special ability

public:
    Spy();
    ~Spy() override;
    
    // Rule of Three
    Spy(const Spy& other);
    Spy& operator=(const Spy& other);
    
    // Spy can view another player's coin count - doesn't consume a turn
    void spyOn(const Player& target, Game& game);
    
    // Can block arrest actions after spying on the player
    bool canBlockArrest(const Player& performer) const override;
};