#include "../../include/Roles/General.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"
#include "../../include/Exceptions.hpp"

// Constructor
General::General() : Role("General") {}

// Destructor
General::~General() {}

// Copy constructor
General::General(const General& other) : Role(other) {}

// Assignment operator
General& General::operator=(const General& other) {
    if (this != &other) {
        Role::operator=(other);
    }
    return *this;
}

// Block a coup targeting another player or self (costs 5 coins)
bool General::canBlockCoup(const Player& performer, const Player& target [[maybe_unused]]) const {
    // Check if the player has enough coins to block a coup
    if (performer.getCoins() >= 5) {
        return true;
    }
    return false;
}

// Regain coins when arrested
void General::onBeingArrested(Player& player, Player& performer [[maybe_unused]]) {
    // General regains the coin that was taken
    player.addCoins(1);
}