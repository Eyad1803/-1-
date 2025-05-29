#pragma once

#include <string>
#include <memory>

// Forward declarations
class Role;
class Game;

class Player {
private:
    std::string name;
    std::unique_ptr<Role> role;
    int coins;
    bool active;
    std::string lastArrestedPlayer; // To track who was last arrested by this player
    bool sanctioned;               // Whether this player is under sanction
    
public:
    Player(const std::string& name, std::unique_ptr<Role> role);
    ~Player();
    
    // Rule of Three implementation
    Player(const Player& other);
    Player& operator=(const Player& other);
    
    // Getters
    std::string getName() const;
    Role* getRole() const;
    int getCoins() const;
    bool isActive() const;
    bool isSanctioned() const;
    std::string getLastArrestedPlayer() const;
    
    // Actions
    void gather(Game& game);           // Take 1 coin from the pot
    void tax(Game& game);              // Take 2 coins from the pot (can be blocked)
    void bribe(Game& game);            // Pay 4 coins for an extra action
    void arrest(Player& target, Game& game);    // Steal 1 coin from another player
    void sanction(Player& target, Game& game);  // Prevent economic actions (costs 3 coins)
    void coup(Player& target, Game& game);      // Eliminate another player (costs 7 coins)
    
    // State modifiers
    void addCoins(int amount);
    void removeCoins(int amount);
    void eliminate();
    void applySanction();
    void removeSanction();
    void setLastArrestedPlayer(const std::string& playerName);
    
    // Checks
    bool mustCoup() const; // Check if player has 10+ coins and must coup
    bool canArrestTarget(const Player& target) const; // Check if player can arrest this target
};