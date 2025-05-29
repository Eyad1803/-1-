#include "../../include/Roles/Judge.hpp"
#include "../../include/Player.hpp"
#include "../../include/Game.hpp"

// Constructor
Judge::Judge() : Role("Judge") {}

// Destructor
Judge::~Judge() {}

// Copy constructor
Judge::Judge(const Judge& other) : Role(other) {}

// Assignment operator
Judge& Judge::operator=(const Judge& other) {
    if (this != &other) {
        Role::operator=(other);
    }
    return *this;
}

// Can block a bribe
bool Judge::canBlockBribe(const Player& performer [[maybe_unused]]) const {
    // Judge can block bribe actions
    return true;
}

// When sanctioned, performer pays 1 additional coin
void Judge::onBeingSanctioned(Player& player [[maybe_unused]], Player& performer) {
    // When a Judge is sanctioned, the performer must pay an additional coin
    performer.removeCoins(1);
}