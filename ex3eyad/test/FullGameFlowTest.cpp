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
#include <iostream>
#include <string>

TEST_CASE("Simplified full game flow") {
    // Create a new game
    Game game;
    
    // Create 4 players with different roles
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    std::unique_ptr<Player> charlie = std::make_unique<Player>("Charlie", std::make_unique<Baron>());
    std::unique_ptr<Player> dave = std::make_unique<Player>("Dave", std::make_unique<General>());
    
    // Store references for easy access
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    Player& charlieRef = *charlie;
    Player& daveRef = *dave;
    
    // Add players to the game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.addPlayer(std::move(charlie));
    game.addPlayer(std::move(dave));
    
    // Start the game
    game.start();
    
    // Initial state checks
    CHECK(game.players().size() == 4);
    CHECK(game.isGameOver() == false);
    CHECK(game.turn() == "Alice");
    
    CHECK(aliceRef.getCoins() == 0);
    CHECK(bobRef.getCoins() == 0);
    CHECK(charlieRef.getCoins() == 0);
    CHECK(daveRef.getCoins() == 0);
    
    // Turn 1: Alice (Governor) gathers
    MESSAGE("Turn 1: Alice gathers");
    aliceRef.gather(game);
    CHECK(aliceRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 2: Bob (Spy) gathers
    MESSAGE("Turn 2: Bob gathers");
    bobRef.gather(game);
    CHECK(bobRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 3: Charlie (Baron) gathers
    MESSAGE("Turn 3: Charlie gathers");
    charlieRef.gather(game);
    CHECK(charlieRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 4: Dave (General) gathers
    MESSAGE("Turn 4: Dave gathers");
    daveRef.gather(game);
    CHECK(daveRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 5: Alice taxes (Governor gets 3 coins)
    MESSAGE("Turn 5: Alice taxes");
    aliceRef.tax(game);
    CHECK(aliceRef.getCoins() == 4);  // 1 + 3 = 4 (Governor gets 3)
    game.nextTurn();
    
    // Turn 6: Bob arrests Charlie
    MESSAGE("Turn 6: Bob arrests Charlie");
    bobRef.arrest(charlieRef, game);
    CHECK(bobRef.getCoins() == 2);     // 1 + 1 = 2
    CHECK(charlieRef.getCoins() == 0); // 1 - 1 = 0
    game.nextTurn();
    
    // Turn 7: Charlie gathers to recover
    MESSAGE("Turn 7: Charlie gathers");
    charlieRef.gather(game);
    CHECK(charlieRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 8: Dave gathers
    MESSAGE("Turn 8: Dave gathers");
    daveRef.gather(game);
    CHECK(daveRef.getCoins() == 2);
    game.nextTurn();
    
    // Turn 9: Alice performs coup on Charlie
    MESSAGE("Turn 9: Alice coups Charlie");
    aliceRef.addCoins(3);  // Add coins for testing purposes
    CHECK(aliceRef.getCoins() == 7);   // 4 + 3 = 7
    aliceRef.coup(charlieRef, game);
    CHECK(aliceRef.getCoins() == 0);   // 7 - 7 = 0
    CHECK(charlieRef.isActive() == false);
    CHECK(game.players().size() == 3);
    game.nextTurn();
    
    // Turn 10: Bob gathers
    MESSAGE("Turn 10: Bob gathers");
    bobRef.gather(game);
    CHECK(bobRef.getCoins() == 3);
    game.nextTurn();
    
    // Charlie was eliminated, so Dave's turn
    
    // Turn 11: Dave gathers
    MESSAGE("Turn 11: Dave gathers");
    daveRef.gather(game);
    CHECK(daveRef.getCoins() == 3);
    game.nextTurn();
    
    // Turn 12: Alice gathers
    MESSAGE("Turn 12: Alice gathers");
    aliceRef.gather(game);
    CHECK(aliceRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 13: Bob arrests Dave
    MESSAGE("Turn 13: Bob arrests Dave");
    bobRef.arrest(daveRef, game);
    CHECK(bobRef.getCoins() == 4);    // 3 + 1 = 4
    CHECK(daveRef.getCoins() == 3);   // Value depends on implementation
    game.nextTurn();

    // Turn 14: Dave gathers
    MESSAGE("Turn 14: Dave gathers");
    daveRef.gather(game);
    CHECK(daveRef.getCoins() == 4);   // Value depends on implementation
    game.nextTurn();
    
    // Turn 15: Alice gathers
    MESSAGE("Turn 15: Alice gathers");
    aliceRef.gather(game);
    CHECK(aliceRef.getCoins() == 2);
    game.nextTurn();
    
    // Turn 16: Bob performs coup on Dave
    MESSAGE("Turn 16: Bob coups Dave");
    bobRef.addCoins(3);  // Add coins for testing purposes
    CHECK(bobRef.getCoins() == 7);    // 4 + 3 = 7
    bobRef.coup(daveRef, game);
    CHECK(bobRef.getCoins() == 0);    // 7 - 7 = 0
    CHECK(daveRef.isActive() == false);
    CHECK(game.players().size() == 2);
    game.nextTurn();
    
    // Only Alice and Bob remain
    
    // Turn 17: Alice gathers
    MESSAGE("Turn 17: Alice gathers");
    aliceRef.gather(game);
    CHECK(aliceRef.getCoins() == 3);
    game.nextTurn();
    
    // Turn 18: Bob gathers
    MESSAGE("Turn 18: Bob gathers");
    bobRef.gather(game);
    CHECK(bobRef.getCoins() == 1);
    game.nextTurn();
    
    // Turn 19: Alice taxes (Governor gets 3 coins)
    MESSAGE("Turn 19: Alice taxes");
    aliceRef.tax(game);
    CHECK(aliceRef.getCoins() == 6);  // 3 + 3 = 6
    game.nextTurn();
    
    // Turn 20: Bob gathers
    MESSAGE("Turn 20: Bob gathers");
    bobRef.gather(game);
    CHECK(bobRef.getCoins() == 2);
    game.nextTurn();
    
    // Turn 21: Alice performs coup on Bob
    MESSAGE("Turn 21: Alice coups Bob");
    aliceRef.addCoins(1);  // Add coins for testing purposes
    CHECK(aliceRef.getCoins() == 7);  // 6 + 1 = 7
    aliceRef.coup(bobRef, game);
    CHECK(aliceRef.getCoins() == 0);  // 7 - 7 = 0
    CHECK(bobRef.isActive() == false);
    
    // Game should be over, Alice wins
    CHECK(game.isGameOver() == true);
    CHECK(game.players().size() == 1);
    CHECK(game.players()[0] == "Alice");
    CHECK(game.winner() == "Alice");
}

TEST_CASE("Game rules and exceptions") {
    Game game;
    
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Cannot arrest same player twice in a row") {
        // Alice arrests Bob
        aliceRef.addCoins(1);  // Ensure Alice has a coin
        aliceRef.arrest(bobRef, game);
        
        // End turn, go to Bob
        game.nextTurn();
        
        // Bob gathers and ends turn
        bobRef.gather(game);
        game.nextTurn();
        
        // Now Alice tries to arrest Bob again
        CHECK_THROWS_AS(aliceRef.arrest(bobRef, game), Coup::ConsecutiveArrestException);
    }
    
    SUBCASE("Must coup with 10+ coins") {
        // Give Alice 10 coins
        aliceRef.addCoins(10);
        CHECK(aliceRef.getCoins() == 10);
        
        // Alice must coup, cannot gather
        CHECK_THROWS_AS(aliceRef.gather(game), Coup::MandatoryCoupException);
        CHECK_THROWS_AS(aliceRef.tax(game), Coup::MandatoryCoupException);
        
        // Only coup is allowed
        aliceRef.coup(bobRef, game);
        
        // Game should be over
        CHECK(game.isGameOver() == true);
    }
    
    SUBCASE("Cannot perform economic actions while sanctioned") {
        // Bob sanctions Alice
        bobRef.addCoins(3);
        bobRef.sanction(aliceRef, game);
        
        // Alice is sanctioned and can't perform economic actions
        CHECK(aliceRef.isSanctioned() == true);
        CHECK_THROWS_AS(aliceRef.gather(game), Coup::SanctionedException);
        CHECK_THROWS_AS(aliceRef.tax(game), Coup::SanctionedException);
        
        // But can still perform non-economic actions
        aliceRef.addCoins(7);  // Add coins for testing
        CHECK_NOTHROW(aliceRef.coup(bobRef, game));
    }
    
    SUBCASE("Insufficient coins for actions") {
        // Bribe requires 4 coins
        CHECK_THROWS_AS(aliceRef.bribe(game), Coup::InsufficientCoinsException);
        
        // Sanction requires 3 coins
        CHECK_THROWS_AS(aliceRef.sanction(bobRef, game), Coup::InsufficientCoinsException);
        
        // Coup requires 7 coins
        CHECK_THROWS_AS(aliceRef.coup(bobRef, game), Coup::InsufficientCoinsException);
    }
}

TEST_CASE("Role-specific abilities") {
    Game game;
    
    SUBCASE("Governor - tax gives 3 coins") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
        game.addPlayer(std::move(alice));
        game.addPlayer(std::make_unique<Player>("Bob", std::make_unique<Spy>())); // Need 2 players
        game.start();
        
        Player& aliceRef = game.getPlayer("Alice");
        
        aliceRef.tax(game);
        CHECK(aliceRef.getCoins() == 3);  // Governor gets 3 coins from tax
    }
    
    SUBCASE("Merchant - gather gives 1 coin") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Merchant>());
        game.addPlayer(std::move(alice));
        game.addPlayer(std::make_unique<Player>("Bob", std::make_unique<Spy>())); // Need 2 players
        game.start();
        
        Player& aliceRef = game.getPlayer("Alice");
        
        aliceRef.gather(game);
        CHECK(aliceRef.getCoins() == 1);  // Merchant gets 1 coin from gather
    }
    
    SUBCASE("Baron - invest ability") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Baron>());
        game.addPlayer(std::move(alice));
        game.addPlayer(std::make_unique<Player>("Bob", std::make_unique<Spy>())); // Need 2 players
        game.start();
        
        Player& aliceRef = game.getPlayer("Alice");
        Baron* baronRole = dynamic_cast<Baron*>(aliceRef.getRole());
        
        // Need 3 coins to invest
        aliceRef.addCoins(3);
        baronRole->invest(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 6);  // 3 - 3 + 6 = 6
    }
    
    SUBCASE("General - arrest takes 1 coin") {
        std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<General>());
        std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
        
        Player& bobRef = *bob;
        
        game.addPlayer(std::move(alice));
        game.addPlayer(std::move(bob));
        game.start();
        
        Player& aliceRef = game.getPlayer("Alice");
        
        // Give Bob some coins
        bobRef.addCoins(5);
        CHECK(bobRef.getCoins() == 5);
        
        // Alice (General) arrests Bob
        aliceRef.arrest(bobRef, game);
        
        CHECK(aliceRef.getCoins() == 1);  // Takes 1 coin
        CHECK(bobRef.getCoins() == 4);    // Bob loses 1 coin
    }
}