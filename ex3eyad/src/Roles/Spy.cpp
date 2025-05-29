#include "../../include/Roles/Spy.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include <iostream>

// Constructor
Spy::Spy() : Role("Spy"), hasSpiedOnPlayer(false) {}

// Destructor
Spy::~Spy() {}

// Copy constructor
Spy::Spy(const Spy& other) : Role(other), hasSpiedOnPlayer(other.hasSpiedOnPlayer) {}

// Assignment operator
Spy& Spy::operator=(const Spy& other) {
    if (this != &other) {
        Role::operator=(other);
        hasSpiedOnPlayer = other.hasSpiedOnPlayer;
    }
    return *this;
}

// Spy can view another player's coin count - doesn't consume a turn
void Spy::spyOn(const Player& target, Game& game [[maybe_unused]]) {
    // Simply get the coin count and display it (in a real game, this would be shown to the player)
    std::cout << "Spy: " << target.getName() << " has " << target.getCoins() << " coins." << std::endl;
    hasSpiedOnPlayer = true;
}

// Can block arrest actions after spying on the player
bool Spy::canBlockArrest(const Player& performer [[maybe_unused]]) const {
    // Spy can block arrest actions if they have spied on a player
    return hasSpiedOnPlayer;
}