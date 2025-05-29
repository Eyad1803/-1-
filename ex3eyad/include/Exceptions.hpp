#pragma once

#include <stdexcept>
#include <string>

namespace Coup {
    // Base exception class for all Coup game exceptions
    class CoupException : public std::runtime_error {
    public:
        CoupException(const std::string& message) : std::runtime_error(message) {}
    };
    
    // Player doesn't have enough coins for action
    class InsufficientCoinsException : public CoupException {
    public:
        InsufficientCoinsException(const std::string& message) : CoupException(message) {}
    };
    
    // Action is blocked by another player's role
    class ActionBlockedException : public CoupException {
    public:
        ActionBlockedException(const std::string& message) : CoupException(message) {}
    };
    
    // Trying to target a player who is no longer active
    class InvalidTargetException : public CoupException {
    public:
        InvalidTargetException(const std::string& message) : CoupException(message) {}
    };
    
    // Trying to arrest same player twice in a row
    class ConsecutiveArrestException : public CoupException {
    public:
        ConsecutiveArrestException(const std::string& message) : CoupException(message) {}
    };
    
    // Player is under sanction and can't perform economic actions
    class SanctionedException : public CoupException {
    public:
        SanctionedException(const std::string& message) : CoupException(message) {}
    };
    
    // Player with 10+ coins must perform coup
    class MandatoryCoupException : public CoupException {
    public:
        MandatoryCoupException(const std::string& message) : CoupException(message) {}
    };
    
    // Game is over, no more actions allowed
    class GameOverException : public CoupException {
    public:
        GameOverException(const std::string& message) : CoupException(message) {}
    };
    
    // No winner yet, game still in progress
    class GameInProgressException : public CoupException {
    public:
        GameInProgressException(const std::string& message) : CoupException(message) {}
    };
}