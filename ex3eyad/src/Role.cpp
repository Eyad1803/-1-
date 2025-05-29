#include "../include/Role.hpp"
#include "../include/Player.hpp"
#include "../include/Game.hpp"

// Constructor
Role::Role(const std::string& name) : name(name) {}

// Destructor
Role::~Role() {}

// Copy constructor
Role::Role(const Role& other) : name(other.name) {}

// Assignment operator
Role& Role::operator=(const Role& other) {
    if (this != &other) {
        name = other.name;
    }
    return *this;
}

// Getters
std::string Role::getName() const {
    return name;
}

// Default implementations for action modifiers - can be overridden
void Role::onGather(Player& player [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default gather: No special behavior
}

void Role::onTax(Player& player [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default tax: No special behavior
}

void Role::onBribe(Player& player [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default bribe: No special behavior
}

void Role::onArrest(Player& player [[maybe_unused]], Player& target [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default arrest: No special behavior
}

void Role::onSanction(Player& player [[maybe_unused]], Player& target [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default sanction: No special behavior
}

void Role::onCoup(Player& player [[maybe_unused]], Player& target [[maybe_unused]], Game& game [[maybe_unused]]) {
    // Default coup: No special behavior
}

// Default implementations for blocking actions - can be overridden
bool Role::canBlockTax(const Player& performer [[maybe_unused]]) const {
    // Default: Cannot block tax
    return false;
}

bool Role::canBlockArrest(const Player& performer [[maybe_unused]]) const {
    // Default: Cannot block arrest
    return false;
}

bool Role::canBlockBribe(const Player& performer [[maybe_unused]]) const {
    // Default: Cannot block bribe
    return false;
}

bool Role::canBlockCoup(const Player& performer [[maybe_unused]], const Player& target [[maybe_unused]]) const {
    // Default: Cannot block coup
    return false;
}

// Default implementations for reactions to being targeted - can be overridden
void Role::onBeingArrested(Player& player [[maybe_unused]], Player& performer [[maybe_unused]]) {
    // Default: No special reaction to being arrested
}

void Role::onBeingSanctioned(Player& player [[maybe_unused]], Player& performer [[maybe_unused]]) {
    // Default: No special reaction to being sanctioned
}