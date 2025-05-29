#include "../include/Player.hpp"
#include "../include/Role.hpp"
#include "../include/Game.hpp"
#include "../include/Roles/Governor.hpp"
#include "../include/Roles/Spy.hpp"
#include "../include/Roles/Baron.hpp"
#include "../include/Roles/General.hpp"
#include "../include/Roles/Judge.hpp"
#include "../include/Roles/Merchant.hpp"
#include "../include/Exceptions.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    std::cout << "Welcome to Coup!" << std::endl;
    std::cout << "This is a command-line version of the game for demonstration purposes." << std::endl;
    std::cout << "A GUI version would be implemented using Qt in a complete implementation." << std::endl;
    std::cout << "----------------" << std::endl;
    
    // Create a new game
    Game game;
    
    // Ask for number of players
    int numPlayers;
    std::cout << "Enter number of players (2-6): ";
    std::cin >> numPlayers;
    
    // Validate input
    if (numPlayers < 2 || numPlayers > 6) {
        std::cout << "Invalid number of players. Defaulting to 4 players." << std::endl;
        numPlayers = 4;
    }
    
    // Create players
    for (int i = 0; i < numPlayers; i++) {
        std::string name;
        int roleChoice;
        
        std::cout << "Enter name for Player " << (i+1) << ": ";
        std::cin >> name;
        
        std::cout << "Choose a role for " << name << ":" << std::endl;
        std::cout << "1. Governor" << std::endl;
        std::cout << "2. Spy" << std::endl;
        std::cout << "3. Baron" << std::endl;
        std::cout << "4. General" << std::endl;
        std::cout << "5. Judge" << std::endl;
        std::cout << "6. Merchant" << std::endl;
        std::cout << "Enter your choice (1-6): ";
        std::cin >> roleChoice;
        
        // Create role based on choice
        std::unique_ptr<Role> role;
        switch (roleChoice) {
            case 1:
                role = std::make_unique<Governor>();
                break;
            case 2:
                role = std::make_unique<Spy>();
                break;
            case 3:
                role = std::make_unique<Baron>();
                break;
            case 4:
                role = std::make_unique<General>();
                break;
            case 5:
                role = std::make_unique<Judge>();
                break;
            case 6:
                role = std::make_unique<Merchant>();
                break;
            default:
                std::cout << "Invalid choice. Defaulting to Governor." << std::endl;
                role = std::make_unique<Governor>();
        }
        
        // Create player and add to game
        std::unique_ptr<Player> player = std::make_unique<Player>(name, std::move(role));
        game.addPlayer(std::move(player));
    }
    
    // Start the game
    game.start();
    
    // Main game loop
    while (!game.isGameOver()) {
        std::string currentPlayer = game.turn();
        Player& player = game.getPlayer(currentPlayer);
        
        // Display game state
        std::cout << "\n----------------" << std::endl;
        std::cout << "Current player: " << currentPlayer << std::endl;
        std::cout << "Role: " << player.getRole()->getName() << std::endl;
        std::cout << "Coins: " << player.getCoins() << std::endl;
        
        // Display all players and their coins
        std::cout << "\nAll players:" << std::endl;
        for (const auto& playerName : game.players()) {
            const Player& p = game.getPlayer(playerName);
            std::cout << playerName << " (" << p.getRole()->getName() 
                     << "): " << p.getCoins() << " coins" << std::endl;
        }
        
        // Display available actions
        std::cout << "\nAvailable actions:" << std::endl;
        std::cout << "1. Gather (take 1 coin)" << std::endl;
        std::cout << "2. Tax (take 2-3 coins depending on role)" << std::endl;
        
        // Bribe (requires 4 coins)
        if (player.getCoins() >= 4) {
            std::cout << "3. Bribe (pay 4 coins for extra action)" << std::endl;
        }
        
        // Arrest (cannot target the same player twice in a row)
        std::cout << "4. Arrest (steal 1 coin from another player)" << std::endl;
        
        // Sanction (requires 3 coins)
        if (player.getCoins() >= 3) {
            std::cout << "5. Sanction (prevent economic actions, costs 3 coins)" << std::endl;
        }
        
        // Coup (requires 7 coins, mandatory with 10+ coins)
        if (player.getCoins() >= 7) {
            std::cout << "6. Coup (eliminate player, costs 7 coins)";
            if (player.mustCoup()) {
                std::cout << " - MANDATORY";
            }
            std::cout << std::endl;
        }
        
        // Role-specific actions
        if (player.getRole()->getName() == "Baron" && player.getCoins() >= 3) {
            std::cout << "7. Invest (Baron: pay 3 coins to get 6)" << std::endl;
        }
        
        if (player.getRole()->getName() == "Spy") {
            std::cout << "8. Spy on (Spy: view another player's coins)" << std::endl;
        }
        
        // Get player action
        int actionChoice;
        std::cout << "\nEnter your choice: ";
        std::cin >> actionChoice;
        
        try {
            // Process action
            switch (actionChoice) {
                case 1: // Gather
                    player.gather(game);
                    std::cout << player.getName() << " gathered 1 coin." << std::endl;
                    break;
                    
                case 2: // Tax
                    player.tax(game);
                    if (player.getRole()->getName() == "Governor") {
                        std::cout << player.getName() << " taxed 3 coins." << std::endl;
                    } else {
                        std::cout << player.getName() << " taxed 2 coins." << std::endl;
                    }
                    break;
                    
                case 3: // Bribe
                    if (player.getCoins() >= 4) {
                        player.bribe(game);
                        std::cout << player.getName() << " bribed for an extra action." << std::endl;
                        
                        // Perform an extra action (simplified for demo)
                        std::cout << "Extra action: Gather 1 coin" << std::endl;
                        player.gather(game);
                    } else {
                        std::cout << "Not enough coins to bribe." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                    
                case 4: // Arrest
                {
                    // Choose target
                    std::cout << "Choose a player to arrest:" << std::endl;
                    std::vector<std::string> playerNames = game.players();
                    
                    for (size_t i = 0; i < playerNames.size(); i++) {
                        if (playerNames[i] != currentPlayer) {
                            std::cout << i << ". " << playerNames[i] << std::endl;
                        }
                    }
                    
                    int targetIndex;
                    std::cout << "Enter target index: ";
                    std::cin >> targetIndex;
                    
                    if (targetIndex >= 0 && targetIndex < (int)playerNames.size() && 
                        playerNames[targetIndex] != currentPlayer) {
                        Player& target = game.getPlayer(playerNames[targetIndex]);
                        player.arrest(target, game);
                        std::cout << player.getName() << " arrested " << target.getName() 
                                 << ", stealing 1 coin." << std::endl;
                    } else {
                        std::cout << "Invalid target." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                }
                    
                case 5: // Sanction
                {
                    if (player.getCoins() >= 3) {
                        // Choose target
                        std::cout << "Choose a player to sanction:" << std::endl;
                        std::vector<std::string> playerNames = game.players();
                        
                        for (size_t i = 0; i < playerNames.size(); i++) {
                            if (playerNames[i] != currentPlayer) {
                                std::cout << i << ". " << playerNames[i] << std::endl;
                            }
                        }
                        
                        int targetIndex;
                        std::cout << "Enter target index: ";
                        std::cin >> targetIndex;
                        
                        if (targetIndex >= 0 && targetIndex < (int)playerNames.size() && 
                            playerNames[targetIndex] != currentPlayer) {
                            Player& target = game.getPlayer(playerNames[targetIndex]);
                            player.sanction(target, game);
                            std::cout << player.getName() << " sanctioned " << target.getName() << "." << std::endl;
                        } else {
                            std::cout << "Invalid target." << std::endl;
                            continue; // Don't end turn yet
                        }
                    } else {
                        std::cout << "Not enough coins to sanction." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                }
                    
                case 6: // Coup
                {
                    if (player.getCoins() >= 7) {
                        // Choose target
                        std::cout << "Choose a player to coup:" << std::endl;
                        std::vector<std::string> playerNames = game.players();
                        
                        for (size_t i = 0; i < playerNames.size(); i++) {
                            if (playerNames[i] != currentPlayer) {
                                std::cout << i << ". " << playerNames[i] << std::endl;
                            }
                        }
                        
                        int targetIndex;
                        std::cout << "Enter target index: ";
                        std::cin >> targetIndex;
                        
                        if (targetIndex >= 0 && targetIndex < (int)playerNames.size() && 
                            playerNames[targetIndex] != currentPlayer) {
                            Player& target = game.getPlayer(playerNames[targetIndex]);
                            player.coup(target, game);
                            std::cout << player.getName() << " couped " << target.getName() << "!" << std::endl;
                        } else {
                            std::cout << "Invalid target." << std::endl;
                            continue; // Don't end turn yet
                        }
                    } else {
                        std::cout << "Not enough coins to coup." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                }
                    
                case 7: // Baron's invest
                {
                    if (player.getRole()->getName() == "Baron" && player.getCoins() >= 3) {
                        Baron* baronRole = dynamic_cast<Baron*>(player.getRole());
                        baronRole->invest(player, game);
                        std::cout << player.getName() << " invested 3 coins to receive 6." << std::endl;
                    } else {
                        std::cout << "Cannot use this action." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                }
                    
                case 8: // Spy's ability
                {
                    if (player.getRole()->getName() == "Spy") {
                        // Choose target
                        std::cout << "Choose a player to spy on:" << std::endl;
                        std::vector<std::string> playerNames = game.players();
                        
                        for (size_t i = 0; i < playerNames.size(); i++) {
                            if (playerNames[i] != currentPlayer) {
                                std::cout << i << ". " << playerNames[i] << std::endl;
                            }
                        }
                        
                        int targetIndex;
                        std::cout << "Enter target index: ";
                        std::cin >> targetIndex;
                        
                        if (targetIndex >= 0 && targetIndex < (int)playerNames.size() && 
                            playerNames[targetIndex] != currentPlayer) {
                            Player& target = game.getPlayer(playerNames[targetIndex]);
                            Spy* spyRole = dynamic_cast<Spy*>(player.getRole());
                            spyRole->spyOn(target, game);
                            std::cout << target.getName() << " has " << target.getCoins() << " coins." << std::endl;
                        } else {
                            std::cout << "Invalid target." << std::endl;
                            continue; // Don't end turn yet
                        }
                    } else {
                        std::cout << "Cannot use this action." << std::endl;
                        continue; // Don't end turn yet
                    }
                    break;
                }
                    
                default:
                    std::cout << "Invalid choice. Please try again." << std::endl;
                    continue; // Don't end turn yet
            }
            
            // Move to next player's turn
            game.nextTurn();
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Please try again." << std::endl;
        }
    }
    
    // Game over
    std::cout << "\n----------------" << std::endl;
    std::cout << "Game over!" << std::endl;
    std::cout << "The winner is: " << game.winner() << std::endl;
    
    return 0;
}