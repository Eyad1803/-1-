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

int main() {
    std::cout << "Welcome to Coup!" << std::endl;
    std::cout << "----------------" << std::endl;
    
    // Create a new game
    Game game;
    
    // Create players with different roles
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    std::unique_ptr<Player> charlie = std::make_unique<Player>("Charlie", std::make_unique<Baron>());
    std::unique_ptr<Player> david = std::make_unique<Player>("David", std::make_unique<General>());
    
    // Keep references for demonstration
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    Player& charlieRef = *charlie;
    Player& davidRef = *david;
    
    // Add players to the game
    std::cout << "Adding players to the game..." << std::endl;
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.addPlayer(std::move(charlie));
    game.addPlayer(std::move(david));
    
    // Start the game
    std::cout << "Starting the game..." << std::endl;
    game.start();
    
    // Print initial state
    std::cout << "Initial players: ";
    for (const auto& player : game.players()) {
        std::cout << player << " ";
    }
    std::cout << std::endl;
    
    // Run a demo scenario
    try {
        std::cout << "Turn 1: " << game.turn() << " (Alice - Governor)" << std::endl;
        std::cout << "Alice taxes (gets 3 coins as Governor)" << std::endl;
        aliceRef.tax(game);
        std::cout << "Alice now has " << aliceRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 2: " << game.turn() << " (Bob - Spy)" << std::endl;
        std::cout << "Bob taxes (gets 2 coins)" << std::endl;
        bobRef.tax(game);
        std::cout << "Bob now has " << bobRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 3: " << game.turn() << " (Charlie - Baron)" << std::endl;
        std::cout << "Charlie taxes (gets 2 coins)" << std::endl;
        charlieRef.tax(game);
        std::cout << "Charlie now has " << charlieRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 4: " << game.turn() << " (David - General)" << std::endl;
        std::cout << "David taxes (gets 2 coins)" << std::endl;
        davidRef.tax(game);
        std::cout << "David now has " << davidRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 5: " << game.turn() << " (Alice - Governor)" << std::endl;
        std::cout << "Alice taxes again (gets 3 more coins)" << std::endl;
        aliceRef.tax(game);
        std::cout << "Alice now has " << aliceRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 6: " << game.turn() << " (Bob - Spy)" << std::endl;
        std::cout << "Bob arrests Charlie" << std::endl;
        bobRef.arrest(charlieRef, game);
        std::cout << "Bob now has " << bobRef.getCoins() << " coins" << std::endl;
        std::cout << "Charlie now has " << charlieRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 7: " << game.turn() << " (Charlie - Baron)" << std::endl;
        std::cout << "Charlie uses Baron's invest ability (pays 3, gets 6)" << std::endl;
        Baron* baronRole = dynamic_cast<Baron*>(charlieRef.getRole());
        baronRole->invest(charlieRef, game);
        std::cout << "Charlie now has " << charlieRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 8: " << game.turn() << " (David - General)" << std::endl;
        std::cout << "David sanctions Bob (costs 3 coins)" << std::endl;
        davidRef.sanction(bobRef, game);
        std::cout << "David now has " << davidRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        std::cout << "\nTurn 9: " << game.turn() << " (Alice - Governor)" << std::endl;
        std::cout << "Alice coups Charlie (costs 7 coins)" << std::endl;
        aliceRef.coup(charlieRef, game);
        std::cout << "Alice now has " << aliceRef.getCoins() << " coins" << std::endl;
        
        // Print remaining players
        std::cout << "\nRemaining players: ";
        for (const auto& player : game.players()) {
            std::cout << player << " ";
        }
        std::cout << std::endl;
        
        game.nextTurn();
        
        std::cout << "\nTurn 10: " << game.turn() << " (Bob - Spy)" << std::endl;
        std::cout << "Bob tries to tax but is sanctioned" << std::endl;
        try {
            bobRef.tax(game);
        } catch (const Coup::SanctionedException& e) {
            std::cout << "Exception caught: " << e.what() << std::endl;
        }
        // Sanction is removed at beginning of Bob's turn
        std::cout << "Bob is no longer sanctioned" << std::endl;
        std::cout << "Bob gathers instead" << std::endl;
        bobRef.gather(game);
        std::cout << "Bob now has " << bobRef.getCoins() << " coins" << std::endl;
        game.nextTurn();
        
        // Continue the game until someone wins
        std::cout << "\nContinuing the game..." << std::endl;
        
        // David coups Bob
        std::cout << "\nTurn 11: " << game.turn() << " (David - General)" << std::endl;
        davidRef.addCoins(5); // Give David enough coins for a coup
        std::cout << "David coups Bob (costs 7 coins)" << std::endl;
        davidRef.coup(bobRef, game);
        
        // Print remaining players
        std::cout << "\nRemaining players: ";
        for (const auto& player : game.players()) {
            std::cout << player << " ";
        }
        std::cout << std::endl;
        
        // At this point, the game should be over with two players eliminated
        std::cout << "\nGame status: " << (game.isGameOver() ? "Over" : "In progress") << std::endl;
        
        if (game.isGameOver()) {
            std::cout << "The winner is: " << game.winner() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}