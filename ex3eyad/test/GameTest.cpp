#include "../include/doctest.h"
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
#include <memory>
#include <vector>

TEST_CASE("Game initialization and basic functionality") {
    Game game;
    
    SUBCASE("Adding players") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
        std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
        
        game.addPlayer(std::move(alice));
        game.addPlayer(std::move(bob));
        
        CHECK(game.players().size() == 2);
        CHECK(game.players()[0] == "Alice");
        CHECK(game.players()[1] == "Bob");
    }
    
    SUBCASE("Current player's turn") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
        std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
        
        game.addPlayer(std::move(alice));
        game.addPlayer(std::move(bob));
        game.start();
        
        CHECK(game.turn() == "Alice");
        
        game.nextTurn();
        CHECK(game.turn() == "Bob");
        
        game.nextTurn();
        CHECK(game.turn() == "Alice");
    }
    
    SUBCASE("Player elimination") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
        std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
        std::unique_ptr<Player> charlie = std::make_unique<Player>("Charlie", std::make_unique<Baron>());
        
        game.addPlayer(std::move(alice));
        game.addPlayer(std::move(bob));
        game.addPlayer(std::move(charlie));
        game.start();
        
        // Initial state
        CHECK(game.players().size() == 3);
        CHECK(game.isGameOver() == false);
        
        // Eliminate Bob
        game.eliminatePlayer("Bob");
        
        // Should have 2 active players
        CHECK(game.players().size() == 2);
        CHECK(game.players()[0] == "Alice");
        CHECK(game.players()[1] == "Charlie");
        CHECK(game.isGameOver() == false);
        
        // Eliminate Charlie
        game.eliminatePlayer("Charlie");
        
        // Should have 1 active player and game should be over
        CHECK(game.players().size() == 1);
        CHECK(game.players()[0] == "Alice");
        CHECK(game.isGameOver() == true);
        CHECK(game.winner() == "Alice");
    }
    
    SUBCASE("Winner when game not over") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
        std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
        
        game.addPlayer(std::move(alice));
        game.addPlayer(std::move(bob));
        game.start();
        
        CHECK(game.isGameOver() == false);
        CHECK_THROWS_AS(game.winner(), Coup::GameInProgressException);
    }
}

TEST_CASE("Game turn management") {
    Game game;

    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    std::unique_ptr<Player> charlie = std::make_unique<Player>("Charlie", std::make_unique<Baron>());

    // Keep references, marking unused with [[maybe_unused]]
    [[maybe_unused]] Player& aliceRef = *alice;
    [[maybe_unused]] Player& bobRef = *bob;

    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.addPlayer(std::move(charlie));
    game.start();
    
    SUBCASE("Turn order") {
        CHECK(game.turn() == "Alice");
        
        game.nextTurn();
        CHECK(game.turn() == "Bob");
        
        game.nextTurn();
        CHECK(game.turn() == "Charlie");
        
        game.nextTurn();
        CHECK(game.turn() == "Alice");
    }
    
    SUBCASE("Skip eliminated players") {
        CHECK(game.turn() == "Alice");
        
        // Eliminate Bob
        bobRef.eliminate();
        game.eliminatePlayer("Bob");
        
        game.nextTurn();
        // Should skip Bob and go to Charlie
        CHECK(game.turn() == "Charlie");
        
        game.nextTurn();
        CHECK(game.turn() == "Alice");
    }
    
    SUBCASE("Removing sanctions at turn start") {
        // Apply sanction to Bob
        aliceRef.addCoins(3);
        aliceRef.sanction(bobRef, game);
        
        CHECK(bobRef.isSanctioned() == true);
        
        // Move to Bob's turn
        game.nextTurn();
        
        // Sanction should be removed
        CHECK(bobRef.isSanctioned() == false);
    }
}