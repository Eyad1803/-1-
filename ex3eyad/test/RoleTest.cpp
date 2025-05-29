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

TEST_CASE("Governor role") {
    Game game;

    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Governor>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());

    // Keep references for testing
    Player& aliceRef = *alice;
    // Using [[maybe_unused]] to silence the warning
    [[maybe_unused]] Player& bobRef = *bob;

    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Governor takes 3 coins for tax") {
        CHECK(aliceRef.getCoins() == 0);
        aliceRef.tax(game);
        CHECK(aliceRef.getCoins() == 3);
    }
    
    // Note: we can't fully test blocking tax without implementing an action resolution system
}

TEST_CASE("Spy role") {
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
    
    SUBCASE("Spy can view another player's coins") {
        aliceRef.addCoins(5);
        
        // This would normally display the coin count, but we can't test the output directly
        // Instead we just ensure it can be called without throwing
        Spy* spyRole = dynamic_cast<Spy*>(bobRef.getRole());
        CHECK_NOTHROW(spyRole->spyOn(aliceRef, game));
    }
}

TEST_CASE("Baron role") {
    Game game;
    
    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Baron>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    // Keep references for testing
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Baron cannot invest with insufficient coins") {
        CHECK(aliceRef.getCoins() == 0);
        
        Baron* baronRole = dynamic_cast<Baron*>(aliceRef.getRole());
        CHECK_THROWS_AS(baronRole->invest(aliceRef, game), Coup::InsufficientCoinsException);
    }
    
    SUBCASE("Baron can invest with enough coins") {
        aliceRef.addCoins(3);
        CHECK(aliceRef.getCoins() == 3);
        
        Baron* baronRole = dynamic_cast<Baron*>(aliceRef.getRole());
        baronRole->invest(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 6); // 3 - 3 + 6 = 6
    }
    
    SUBCASE("Baron gets compensation when sanctioned") {
        bobRef.addCoins(3);
        CHECK(aliceRef.getCoins() == 0);
        
        bobRef.sanction(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 1); // Gets 1 coin as compensation
        CHECK(bobRef.getCoins() == 0);   // Paid 3 coins for sanction
    }
}

TEST_CASE("General role") {
    Game game;
    
    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<General>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    // Keep references for testing
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("General regains coin when arrested") {
        aliceRef.addCoins(2);
        bobRef.addCoins(2);
        
        CHECK(aliceRef.getCoins() == 2);
        CHECK(bobRef.getCoins() == 2);
        
        bobRef.arrest(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 2); // Lost 1 coin but regained it
        CHECK(bobRef.getCoins() == 3);   // Gained 1 coin from arrest
    }
}

TEST_CASE("Judge role") {
    Game game;
    
    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Judge>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    // Keep references for testing
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Judge causes performer to pay extra when sanctioned") {
        bobRef.addCoins(5);
        CHECK(aliceRef.getCoins() == 0);
        
        bobRef.sanction(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 0);
        CHECK(bobRef.getCoins() == 1); // Paid 3 coins for sanction + 1 extra
    }
}

TEST_CASE("Merchant role") {
    Game game;
    
    // Add players to the game
    std::unique_ptr<Player> alice = std::make_unique<Player>("Alice", std::make_unique<Merchant>());
    std::unique_ptr<Player> bob = std::make_unique<Player>("Bob", std::make_unique<Spy>());
    
    // Keep references for testing
    Player& aliceRef = *alice;
    Player& bobRef = *bob;
    
    // Add players to game
    game.addPlayer(std::move(alice));
    game.addPlayer(std::move(bob));
    game.start();
    
    SUBCASE("Merchant pays to pot instead of player when arrested") {
        aliceRef.addCoins(5);
        bobRef.addCoins(2);
        
        CHECK(aliceRef.getCoins() == 5);
        CHECK(bobRef.getCoins() == 2);
        
        bobRef.arrest(aliceRef, game);
        
        CHECK(aliceRef.getCoins() == 2); // Lost 3 coins - first 1 coin from arrest, then 2 coins to pot
        CHECK(bobRef.getCoins() == 2);   // Didn't gain coin from arrest
    }
}