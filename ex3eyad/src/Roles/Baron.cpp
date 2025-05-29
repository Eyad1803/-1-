#include "../../include/Roles/Baron.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Exceptions.hpp"

// Constructor
Baron::Baron() : Role("Baron") {}

// Destructor
Baron::~Baron() {}

// Copy constructor
Baron::Baron(const Baron& other) : Role(other) {}

// Assignment operator
Baron& Baron::operator=(const Baron& other) {
    if (this != &other) {
        Role::operator=(other);
    }
    return *this;
}

// Special invest ability: Pay 3 coins to get 6 coins
void Baron::invest(Player& player, Game& game [[maybe_unused]]) {
    // Check if player has enough coins
    if (player.getCoins() < 3) {
        throw Coup::InsufficientCoinsException("Not enough coins to invest. Requires 3 coins.");
    }

    // Pay 3 coins and receive 6 in return (net gain of 3 coins)
    player.removeCoins(3);
    player.addCoins(6);
}

// Get compensation when sanctioned
void Baron::onBeingSanctioned(Player& player, Player& performer [[maybe_unused]]) {
    // Baron gets 1 coin as compensation when sanctioned
    player.addCoins(1);
}