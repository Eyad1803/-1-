#include "../../include/Roles/Governor.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"

// Constructor
Governor::Governor() : Role("Governor") {}

// Destructor
Governor::~Governor() {}

// Copy constructor
Governor::Governor(const Governor& other) : Role(other) {}

// Assignment operator
Governor& Governor::operator=(const Governor& other) {
    if (this != &other) {
        Role::operator=(other);
    }
    return *this;
}

// Override tax to take 3 coins instead of 2
void Governor::onTax(Player& player, Game& game [[maybe_unused]]) {
    // Governor gets 3 coins for tax instead of 2
    player.addCoins(1); // Add 1 extra coin (base Player::tax already adds 2)
}

// Can block tax actions by others
bool Governor::canBlockTax(const Player& performer [[maybe_unused]]) const {
    // Governor can block other players' tax actions
    return true;
}