# Coup Game Implementation

## Overview
This is a C++ implementation of the Coup board game, where 2-6 players compete to be the last one standing. The implementation includes different roles with unique abilities, various game actions, and a graphical user interface built with Qt.

## Class Structure

### Core Classes
- **Game**: Manages the game state, player turns, and game progression
- **Player**: Represents a player with a role, coins, and various actions
- **Role**: Base class for all roles with default implementations
  
### Roles
- **Governor**: Takes 3 coins for tax and can block tax actions
- **Spy**: Can view other players' coins and block arrests
- **Baron**: Can invest coins for profit and gets compensation when sanctioned
- **General**: Can block coups and regains coins when arrested
- **Judge**: Can block bribes and causes sanctioners to pay extra
- **Merchant**: Gets bonus coins and pays to pot instead of players when arrested

### Actions
- Gather: Take 1 coin
- Tax: Take 2-3 coins (depending on role)
- Bribe: Pay 4 coins for an extra action
- Arrest: Steal 1 coin from another player
- Sanction: Prevent economic actions (costs 3 coins)
- Coup: Eliminate another player (costs 7 coins)

## Building the Project

### Prerequisites
- C++14 compatible compiler
- Qt5 for the GUI version
- doctest for unit testing
- valgrind for memory leak checking

### Compilation
The project includes a Makefile with the following targets:

```
make Main      # Build command-line version
make GUI       # Build graphical version
make test      # Run unit tests
make valgrind  # Check for memory leaks
make clean     # Clean build files
```

## Running the Game
After building, you can run either:
- `./build/Main` for the command-line version
- `./build/GUI` for the graphical version

## Testing
The project includes comprehensive tests for all game components using the doctest framework. Run:
```
make test
```

## Memory Leak Checking
To check for memory leaks, run:
```
make valgrind
```

## Implementation Notes
- The Rule of Three is implemented for all classes with resource ownership
- Virtual inheritance is used for the role hierarchy
- Exception handling is used for illegal actions and game state errors

## GUI
The graphical interface built with Qt provides:
- Game setup screen for adding players
- Game play screen with action buttons
- Player status display
- Target selection for player interactions
- Turn-based gameplay tracking

