#pragma once

#include <string>

// Forward declarations
class Player;
class Game;

class Role {
protected:
    std::string name;

public:
    Role(const std::string& name);
    virtual ~Role();
    
    // Rule of Three implementation
    Role(const Role& other);
    Role& operator=(const Role& other);
    
    // Getters
    std::string getName() const;
    
    // Role-specific action modifiers - can be overridden by derived roles
    virtual void onGather(Player& player, Game& game);
    virtual void onTax(Player& player, Game& game);
    virtual void onBribe(Player& player, Game& game);
    virtual void onArrest(Player& player, Player& target, Game& game);
    virtual void onSanction(Player& player, Player& target, Game& game);
    virtual void onCoup(Player& player, Player& target, Game& game);
    
    // Role-specific reactions to actions by others
    virtual bool canBlockTax(const Player& performer) const;
    virtual bool canBlockArrest(const Player& performer) const;
    virtual bool canBlockBribe(const Player& performer) const;
    virtual bool canBlockCoup(const Player& performer, const Player& target) const;
    
    // Role-specific reactions to being targeted
    virtual void onBeingArrested(Player& player, Player& performer);
    virtual void onBeingSanctioned(Player& player, Player& performer);
};