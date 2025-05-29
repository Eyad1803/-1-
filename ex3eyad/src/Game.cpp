#include "../include/Game.hpp"
#include "../include/Player.hpp"
#include "../include/Exceptions.hpp"

// Constructor
Game::Game() : m_currentPlayerIndex(0), m_gameEnded(false) {}

// Destructor
Game::~Game() {}

// Game initialization
void Game::addPlayer(std::unique_ptr<Player> player) {
    // Cannot add players once the game has started
    if (!m_players.empty() && m_currentPlayerIndex > 0) {
        throw GameException("Cannot add players after the game has started.");
    }
    
    // Store player name for quick lookup
    std::string name = player->getName();
    m_playerIndices[name] = m_players.size();
    
    // Add the player
    m_players.push_back(std::move(player));
}

void Game::start() {
    // Need at least 2 players to start
    if (m_players.size() < 2) {
        throw GameException("Cannot start game with fewer than 2 players.");
    }
    
    // Initialize the game (if needed)
    m_currentPlayerIndex = 0;
    m_gameEnded = false;
}

// Game state queries
std::string Game::turn() const {
    if (m_players.empty()) {
        throw GameException("Game has not been initialized with players.");
    }
    
    return m_players[m_currentPlayerIndex]->getName();
}

std::vector<std::string> Game::players() const {
    std::vector<std::string> playerNames;
    
    for (const auto& player : m_players) {
        if (player->isActive()) {
            playerNames.push_back(player->getName());
        }
    }
    
    return playerNames;
}

std::string Game::winner() const {
    if (!m_gameEnded) {
        throw Coup::GameInProgressException("Game is still in progress, no winner yet.");
    }
    
    // Find the last active player
    for (const auto& player : m_players) {
        if (player->isActive()) {
            return player->getName();
        }
    }
    
    // Should never reach here if game is over
    throw GameException("No active players found, but game is marked as ended.");
}

Player& Game::getCurrentPlayer() {
    if (m_players.empty()) {
        throw GameException("Game has not been initialized with players.");
    }
    
    if (m_gameEnded) {
        throw Coup::GameOverException("Game is over, no current player.");
    }
    
    return *m_players[m_currentPlayerIndex];
}

const Player& Game::getCurrentPlayer() const {
    if (m_players.empty()) {
        throw GameException("Game has not been initialized with players.");
    }
    
    if (m_gameEnded) {
        throw Coup::GameOverException("Game is over, no current player.");
    }
    
    return *m_players[m_currentPlayerIndex];
}

Player& Game::getPlayer(const std::string& name) {
    if (m_playerIndices.find(name) == m_playerIndices.end()) {
        throw GameException("Player not found: " + name);
    }
    
    size_t index = m_playerIndices[name];
    return *m_players[index];
}

const Player& Game::getPlayer(const std::string& name) const {
    if (m_playerIndices.find(name) == m_playerIndices.end()) {
        throw GameException("Player not found: " + name);
    }
    
    size_t index = m_playerIndices.at(name);
    return *m_players[index];
}

bool Game::isGameOver() const {
    return m_gameEnded;
}

// Game state changers
void Game::nextTurn() {
    // Check remaining active players
    int activePlayers = 0;
    for (const auto& player : m_players) {
        if (player->isActive()) {
            activePlayers++;
        }
    }
    
    // If only one player remains, game is over
    if (activePlayers <= 1) {
        m_gameEnded = true;
        return;
    }
    
    // Move to the next active player
    do {
        m_currentPlayerIndex = (m_currentPlayerIndex + 1) % m_players.size();
    } while (!m_players[m_currentPlayerIndex]->isActive());
    
    // Remove sanctions at the beginning of the player's turn
    m_players[m_currentPlayerIndex]->removeSanction();
}

void Game::eliminatePlayer(const std::string& playerName) {
    // Find the player and mark as eliminated
    auto it = m_playerIndices.find(playerName);
    if (it != m_playerIndices.end()) {
        size_t index = it->second;
        m_players[index]->eliminate();
        
        // Check if game is over (only one player left)
        int activePlayers = 0;
        for (const auto& player : m_players) {
            if (player->isActive()) {
                activePlayers++;
            }
        }
        
        if (activePlayers <= 1) {
            m_gameEnded = true;
        }
    }
}