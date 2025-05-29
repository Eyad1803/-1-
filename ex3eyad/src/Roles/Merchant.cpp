#include "../../include/Roles/Merchant.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"

// Constructor
Merchant::Merchant() : Role("Merchant") {}

// Destructor
Merchant::~Merchant() {}

// Copy constructor
Merchant::Merchant(const Merchant& other) : Role(other) {}

// Assignment operator
Merchant& Merchant::operator=(const Merchant& other) {
    if (this != &other) {
        Role::operator=(other);
    }
    return *this;
}

// Get extra coin at turn start if has at least 3 coins
void Merchant::onTurnStart(Player& player, Game& game [[maybe_unused]]) {
    // Merchant gets 1 extra coin if they start with at least 3 coins
    if (player.getCoins() >= 3) {
        player.addCoins(1);
    }
}

// Special behavior when arrested
void Merchant::onBeingArrested(Player& player, Player& performer) {
    // Merchant pays 2 coins to the pot instead of giving 1 to the arresting player
    player.removeCoins(2);

    // Undo the default arrest behavior (performer taking 1 coin from player)
    performer.removeCoins(1);
}