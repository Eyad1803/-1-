#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <stdexcept>

// Forward declaration
class Player;

class Game {
private:
    std::vector<std::unique_ptr<Player>> m_players;
    size_t m_currentPlayerIndex;
    bool m_gameEnded;
    std::unordered_map<std::string, size_t> m_playerIndices; // For quick player lookup by name

public:
    Game();
    ~Game();

    // Game initialization
    void addPlayer(std::unique_ptr<Player> player);
    void start(); // Initialize the game after all players are added

    // Game state queries
    std::string turn() const; // Returns name of current player
    std::vector<std::string> players() const; // Returns names of active players
    std::string winner() const; // Returns winner's name or throws if game not over
    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;
    Player& getPlayer(const std::string& name);
    const Player& getPlayer(const std::string& name) const;
    bool isGameOver() const;
    
    // Game state changers
    void nextTurn(); // Move to next player
    void eliminatePlayer(const std::string& playerName);
    
    // Exception class for game operations
    class GameException : public std::runtime_error {
    public:
        GameException(const std::string& message) : std::runtime_error(message) {}
    };
};