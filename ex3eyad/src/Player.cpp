#include "../include/Player.hpp"
#include "../include/Role.hpp"
#include "../include/Game.hpp"
#include "../include/Exceptions.hpp"

// Constructor
Player::Player(const std::string& name, std::unique_ptr<Role> role) 
    : name(name), role(std::move(role)), coins(0), active(true), lastArrestedPlayer(""), sanctioned(false) {}

// Destructor
Player::~Player() {}

// Copy constructor - deep copy of the role
Player::Player(const Player& other) 
    : name(other.name), 
      // Clone the role (assumes each role class has proper copy constructor)
      role(other.role ? std::unique_ptr<Role>(new Role(*other.role)) : nullptr),
      coins(other.coins), 
      active(other.active), 
      lastArrestedPlayer(other.lastArrestedPlayer),
      sanctioned(other.sanctioned) {}

// Assignment operator - deep copy of the role
Player& Player::operator=(const Player& other) {
    if (this != &other) {
        name = other.name;
        // Clone the role (assumes each role class has proper copy constructor)
        role = other.role ? std::unique_ptr<Role>(new Role(*other.role)) : nullptr;
        coins = other.coins;
        active = other.active;
        lastArrestedPlayer = other.lastArrestedPlayer;
        sanctioned = other.sanctioned;
    }
    return *this;
}

// Getters
std::string Player::getName() const {
    return name;
}

Role* Player::getRole() const {
    return role.get();
}

int Player::getCoins() const {
    return coins;
}

bool Player::isActive() const {
    return active;
}

bool Player::isSanctioned() const {
    return sanctioned;
}

std::string Player::getLastArrestedPlayer() const {
    return lastArrestedPlayer;
}

// Actions
void Player::gather(Game& game) {
    // Check if player must coup
    if (mustCoup()) {
        throw Coup::MandatoryCoupException("Player has 10 or more coins and must perform a coup.");
    }
    
    // Check if under sanction
    if (sanctioned) {
        throw Coup::SanctionedException("Player is sanctioned and cannot perform economic actions.");
    }
    
    // Call the role-specific implementation
    role->onGather(*this, game);
    
    // Default implementation: take 1 coin
    addCoins(1);
}

void Player::tax(Game& game) {
    // Check if player must coup
    if (mustCoup()) {
        throw Coup::MandatoryCoupException("Player has 10 or more coins and must perform a coup.");
    }
    
    // Check if under sanction
    if (sanctioned) {
        throw Coup::SanctionedException("Player is sanctioned and cannot perform economic actions.");
    }
    
    // Call the role-specific implementation
    role->onTax(*this, game);
    
    // Default implementation: take 2 coins
    addCoins(2);
}

void Player::bribe(Game& game) {
    // Check if player must coup
    if (mustCoup()) {
        throw Coup::MandatoryCoupException("Player has 10 or more coins and must perform a coup.");
    }
    
    // Check if player has enough coins
    if (coins < 4) {
        throw Coup::InsufficientCoinsException("Not enough coins to bribe. Requires 4 coins.");
    }
    
    // Call the role-specific implementation
    role->onBribe(*this, game);
    
    // Default implementation: pay 4 coins
    removeCoins(4);
}

void Player::arrest(Player& target, Game& game) {
    // Check if player must coup
    if (mustCoup()) {
        throw Coup::MandatoryCoupException("Player has 10 or more coins and must perform a coup.");
    }

    // Check if targeting the same player as last turn
    if (target.getName() == lastArrestedPlayer) {
        throw Coup::ConsecutiveArrestException("Cannot arrest the same player in consecutive turns.");
    }

    // Call the role-specific implementation
    role->onArrest(*this, target, game);

    // Default implementation: take 1 coin from target
    target.removeCoins(1);
    addCoins(1);

    // Update the last arrested player
    lastArrestedPlayer = target.getName();

    // Allow the target's role to react
    target.getRole()->onBeingArrested(target, *this);
}

void Player::sanction(Player& target, Game& game) {
    // Check if player must coup
    if (mustCoup()) {
        throw Coup::MandatoryCoupException("Player has 10 or more coins and must perform a coup.");
    }
    
    // Check if player has enough coins
    if (coins < 3) {
        throw Coup::InsufficientCoinsException("Not enough coins to sanction. Requires 3 coins.");
    }
    
    // Call the role-specific implementation
    role->onSanction(*this, target, game);
    
    // Default implementation: pay 3 coins and sanction target
    removeCoins(3);
    target.applySanction();
    
    // Allow the target's role to react
    target.getRole()->onBeingSanctioned(target, *this);
}

void Player::coup(Player& target, Game& game) {
    // Check if player has enough coins
    if (coins < 7) {
        throw Coup::InsufficientCoinsException("Not enough coins to coup. Requires 7 coins.");
    }
    
    // Call the role-specific implementation
    role->onCoup(*this, target, game);
    
    // Default implementation: pay 7 coins and eliminate target
    removeCoins(7);
    target.eliminate();
    
    // Update the game state
    game.eliminatePlayer(target.getName());
}

// State modifiers
void Player::addCoins(int amount) {
    coins += amount;
}

void Player::removeCoins(int amount) {
    if (amount > coins) {
        coins = 0;
    } else {
        coins -= amount;
    }
}

void Player::eliminate() {
    active = false;
}

void Player::applySanction() {
    sanctioned = true;
}

void Player::removeSanction() {
    sanctioned = false;
}

void Player::setLastArrestedPlayer(const std::string& playerName) {
    lastArrestedPlayer = playerName;
}

// Checks
bool Player::mustCoup() const {
    return coins >= 10;
}

bool Player::canArrestTarget(const Player& target) const {
    return target.getName() != lastArrestedPlayer && target.isActive();
}