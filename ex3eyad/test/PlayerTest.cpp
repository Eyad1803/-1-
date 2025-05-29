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

TEST_CASE("Player basic functionality") {
    Game game;
    std::unique_ptr<Role> role = std::make_unique<Governor>();
    Player player("Alice", std::move(role));
    
    CHECK(player.getName() == "Alice");
    CHECK(player.getCoins() == 0);
    CHECK(player.isActive() == true);
    CHECK(player.getLastArrestedPlayer() == "");
    
    SUBCASE("Adding and removing coins") {
        player.addCoins(5);
        CHECK(player.getCoins() == 5);
        
        player.removeCoins(2);
        CHECK(player.getCoins() == 3);
        
        // Remove more coins than player has
        player.removeCoins(5);
        CHECK(player.getCoins() == 0);
    }
    
    SUBCASE("Player elimination") {
        CHECK(player.isActive() == true);
        player.eliminate();
        CHECK(player.isActive() == false);
    }
    
    SUBCASE("Player must coup with 10+ coins") {
        CHECK(player.mustCoup() == false);
        
        player.addCoins(9);
        CHECK(player.mustCoup() == false);
        
        player.addCoins(1);
        CHECK(player.mustCoup() == true);
    }
}

TEST_CASE("Player actions") {
    Game game;
    
    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    // Keep references for testing
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Gather action") {
        CHECK(aliceRef.getCoins() == 0);
        aliceRef.gather(game);
        CHECK(aliceRef.getCoins() == 1);
    }
    
    SUBCASE("Tax action") {
        CHECK(aliceRef.getCoins() == 0);
        aliceRef.tax(game);
        // Governor gets 3 coins for tax
        CHECK(aliceRef.getCoins() == 3);
        
        // Normal player gets 2 coins for tax
        CHECK(bobRef.getCoins() == 0);
        bobRef.tax(game);
        CHECK(bobRef.getCoins() == 2);
    }
    
    SUBCASE("Bribe action - insufficient coins") {
        CHECK(aliceRef.getCoins() == 0);
        CHECK_THROWS_AS(aliceRef.bribe(game), Coup::InsufficientCoinsException);
    }
    
    SUBCASE("Bribe action - with enough coins") {
        aliceRef.addCoins(5);
        CHECK(aliceRef.getCoins() == 5);
        aliceRef.bribe(game);
        CHECK(aliceRef.getCoins() == 1);
    }
    
    SUBCASE("Arrest action") {
        // Give coins to both players
        aliceRef.addCoins(2);
        bobRef.addCoins(2);
        
        CHECK(aliceRef.getCoins() == 2);
        CHECK(bobRef.getCoins() == 2);
        
        aliceRef.arrest(bobRef, game);
        
        // Alice should gain 1 coin, Bob should lose 1 coin
        CHECK(aliceRef.getCoins() == 3);
        CHECK(bobRef.getCoins() == 1);
        
        // Check that the last arrested player was set
        CHECK(aliceRef.getLastArrestedPlayer() == "Bob");
        
        // Alice shouldn't be able to arrest Bob again in her next turn
        CHECK_THROWS_AS(aliceRef.arrest(bobRef, game), Coup::ConsecutiveArrestException);
    }
    
    SUBCASE("Sanction action - insufficient coins") {
        aliceRef.addCoins(2);
        CHECK_THROWS_AS(aliceRef.sanction(bobRef, game), Coup::InsufficientCoinsException);
    }
    
    SUBCASE("Sanction action - with enough coins") {
        aliceRef.addCoins(3);
        CHECK(aliceRef.getCoins() == 3);
        
        aliceRef.sanction(bobRef, game);
        CHECK(aliceRef.getCoins() == 0);
        CHECK(bobRef.isSanctioned() == true);
        
        // Bob shouldn't be able to perform economic actions
        CHECK_THROWS_AS(bobRef.gather(game), Coup::SanctionedException);
        CHECK_THROWS_AS(bobRef.tax(game), Coup::SanctionedException);
        
        // Moving to Bob's turn should remove the sanction
        game.nextTurn();
        CHECK(bobRef.isSanctioned() == false);
    }
    
    SUBCASE("Coup action - insufficient coins") {
        aliceRef.addCoins(6);
        CHECK_THROWS_AS(aliceRef.coup(bobRef, game), Coup::InsufficientCoinsException);
    }
    
    SUBCASE("Coup action - with enough coins") {
        aliceRef.addCoins(7);
        CHECK(aliceRef.getCoins() == 7);
        
        aliceRef.coup(bobRef, game);
        CHECK(aliceRef.getCoins() == 0);
        CHECK(bobRef.isActive() == false);
        
        // The game should be over with Alice as the winner
        CHECK(game.isGameOver() == true);
        CHECK(game.winner() == "Alice");
    }
    
    SUBCASE("Mandatory coup with 10+ coins") {
        aliceRef.addCoins(10);
        CHECK(aliceRef.getCoins() == 10);
        
        // Any other action should throw an exception
        CHECK_THROWS_AS(aliceRef.gather(game), Coup::MandatoryCoupException);
        CHECK_THROWS_AS(aliceRef.tax(game), Coup::MandatoryCoupException);
        CHECK_THROWS_AS(aliceRef.bribe(game), Coup::MandatoryCoupException);
        CHECK_THROWS_AS(aliceRef.arrest(bobRef, game), Coup::MandatoryCoupException);
        CHECK_THROWS_AS(aliceRef.sanction(bobRef, game), Coup::MandatoryCoupException);
        
        // But coup should work
        aliceRef.coup(bobRef, game);
        CHECK(aliceRef.getCoins() == 3); // 10 - 7 = 3
        CHECK(bobRef.isActive() == false);
    }
}