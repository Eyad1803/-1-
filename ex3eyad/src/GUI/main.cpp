#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGridLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QPalette>
#include <QTableWidget>
#include <QHeaderView>

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

class CoupGUI : public QMainWindow {
public:
    CoupGUI() {
        setWindowTitle("Coup Game");
        resize(800, 600);

        // Main stacked widget to switch between setup and game screens
        stackedWidget = new QStackedWidget(this);
        setCentralWidget(stackedWidget);

        // Setup the setup screen
        setupScreen = new QWidget();
        gameScreen = new QWidget();

        createSetupScreen();
        createGameScreen();

        stackedWidget->addWidget(setupScreen);
        stackedWidget->addWidget(gameScreen);
        stackedWidget->setCurrentWidget(setupScreen);

        // Initialize game
        game = std::make_unique<Game>();

        // Initialize turn state
        actionTakenThisTurn = false;
        hasBribed = false;
    }
    
private:
    void createSetupScreen() {
        QVBoxLayout* setupLayout = new QVBoxLayout(setupScreen);
        
        // Title
        QLabel* titleLabel = new QLabel("Coup Game", setupScreen);
        QFont titleFont = titleLabel->font();
        titleFont.setPointSize(16);
        titleFont.setBold(true);
        titleLabel->setFont(titleFont);
        titleLabel->setAlignment(Qt::AlignCenter);
        setupLayout->addWidget(titleLabel);
        
        // Setup player input
        QGroupBox* playerBox = new QGroupBox("Add Player", setupScreen);
        QHBoxLayout* playerLayout = new QHBoxLayout(playerBox);
        
        nameField = new QLineEdit(setupScreen);
        nameField->setPlaceholderText("Player Name");
        playerLayout->addWidget(nameField);
        
        roleCombo = new QComboBox(setupScreen);
        roleCombo->addItem("Governor");
        roleCombo->addItem("Spy");
        roleCombo->addItem("Baron");
        roleCombo->addItem("General");
        roleCombo->addItem("Judge");
        roleCombo->addItem("Merchant");
        playerLayout->addWidget(roleCombo);
        
        QPushButton* addButton = new QPushButton("Add Player", setupScreen);
        connect(addButton, &QPushButton::clicked, this, &CoupGUI::addPlayer);
        playerLayout->addWidget(addButton);
        
        setupLayout->addWidget(playerBox);
        
        // Player table
        playerTable = new QTableWidget(0, 2, setupScreen);
        playerTable->setHorizontalHeaderLabels({"Player Name", "Role"});
        playerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        playerTable->setSelectionMode(QAbstractItemView::NoSelection);
        playerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        setupLayout->addWidget(playerTable);
        
        // Start game button
        startButton = new QPushButton("Start Game", setupScreen);
        startButton->setEnabled(false); // Disabled until at least 2 players
        connect(startButton, &QPushButton::clicked, this, &CoupGUI::startGame);
        setupLayout->addWidget(startButton);
        
        // Status
        setupStatusLabel = new QLabel("Add at least 2 players to begin", setupScreen);
        setupStatusLabel->setAlignment(Qt::AlignCenter);
        setupLayout->addWidget(setupStatusLabel);
    }
    
    void createGameScreen() {
        QVBoxLayout* gameLayout = new QVBoxLayout(gameScreen);
        
        // Game status area
        QGroupBox* statusBox = new QGroupBox("Game Status", gameScreen);
        QVBoxLayout* statusLayout = new QVBoxLayout(statusBox);
        
        currentPlayerLabel = new QLabel("Current Player: ", gameScreen);
        QFont boldFont = currentPlayerLabel->font();
        boldFont.setBold(true);
        currentPlayerLabel->setFont(boldFont);
        statusLayout->addWidget(currentPlayerLabel);
        
        gameStatusLabel = new QLabel("Game in progress", gameScreen);
        statusLayout->addWidget(gameStatusLabel);
        
        gameLayout->addWidget(statusBox);
        
        // Player stats area - compact display of all players
        QGroupBox* playersBox = new QGroupBox("Players", gameScreen);
        playerStatsLayout = new QGridLayout(playersBox);
        
        // Headers
        QLabel* nameHeader = new QLabel("Name", gameScreen);
        QLabel* roleHeader = new QLabel("Role", gameScreen);
        QLabel* coinsHeader = new QLabel("Coins", gameScreen);
        QLabel* statusHeader = new QLabel("Status", gameScreen);
        
        nameHeader->setFont(boldFont);
        roleHeader->setFont(boldFont);
        coinsHeader->setFont(boldFont);
        statusHeader->setFont(boldFont);
        
        playerStatsLayout->addWidget(nameHeader, 0, 0);
        playerStatsLayout->addWidget(roleHeader, 0, 1);
        playerStatsLayout->addWidget(coinsHeader, 0, 2);
        playerStatsLayout->addWidget(statusHeader, 0, 3);
        
        // Will be populated when game starts
        
        gameLayout->addWidget(playersBox);
        
        // Action area
        actionBox = new QGroupBox("Actions", gameScreen);
        QVBoxLayout* actionLayout = new QVBoxLayout(actionBox);
        
        // Basic actions
        basicActionsBox = new QGroupBox("Basic Actions");
        QGridLayout* basicGrid = new QGridLayout(basicActionsBox);
        
        gatherButton = new QPushButton("Gather (1 coin)");
        taxButton = new QPushButton("Tax (2-3 coins)");
        bribeButton = new QPushButton("Bribe (pay 4 coins)");
        arrestButton = new QPushButton("Arrest (steal 1 coin)");
        sanctionButton = new QPushButton("Sanction (costs 3 coins)");
        coupButton = new QPushButton("Coup (costs 7 coins)");
        
        connect(gatherButton, &QPushButton::clicked, this, &CoupGUI::onGather);
        connect(taxButton, &QPushButton::clicked, this, &CoupGUI::onTax);
        connect(bribeButton, &QPushButton::clicked, this, &CoupGUI::onBribe);
        connect(arrestButton, &QPushButton::clicked, this, &CoupGUI::onArrest);
        connect(sanctionButton, &QPushButton::clicked, this, &CoupGUI::onSanction);
        connect(coupButton, &QPushButton::clicked, this, &CoupGUI::onCoup);
        
        basicGrid->addWidget(gatherButton, 0, 0);
        basicGrid->addWidget(taxButton, 0, 1);
        basicGrid->addWidget(bribeButton, 1, 0);
        basicGrid->addWidget(arrestButton, 1, 1);
        basicGrid->addWidget(sanctionButton, 2, 0);
        basicGrid->addWidget(coupButton, 2, 1);
        
        actionLayout->addWidget(basicActionsBox);
        
        // Special action
        specialActionButton = new QPushButton("Special Action");
        connect(specialActionButton, &QPushButton::clicked, this, &CoupGUI::onSpecialAction);
        actionLayout->addWidget(specialActionButton);
        
        // Target selection
        QHBoxLayout* targetLayout = new QHBoxLayout();
        QLabel* targetLabel = new QLabel("Target:");
        targetCombo = new QComboBox();
        
        targetLayout->addWidget(targetLabel);
        targetLayout->addWidget(targetCombo);
        actionLayout->addLayout(targetLayout);
        
        // End turn
        endTurnButton = new QPushButton("End Turn");
        connect(endTurnButton, &QPushButton::clicked, this, &CoupGUI::onEndTurn);
        actionLayout->addWidget(endTurnButton);
        
        gameLayout->addWidget(actionBox);
    }
    
    void addPlayer() {
        QString name = nameField->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Please enter a player name");
            return;
        }
        
        // Create role based on selection
        std::unique_ptr<Role> role;
        QString roleName = roleCombo->currentText();
        
        if (roleName == "Governor") {
            role = std::make_unique<Governor>();
        } else if (roleName == "Spy") {
            role = std::make_unique<Spy>();
        } else if (roleName == "Baron") {
            role = std::make_unique<Baron>();
        } else if (roleName == "General") {
            role = std::make_unique<General>();
        } else if (roleName == "Judge") {
            role = std::make_unique<Judge>();
        } else if (roleName == "Merchant") {
            role = std::make_unique<Merchant>();
        }
        
        // Add player to game
        std::unique_ptr<Player> player = std::make_unique<Player>(name.toStdString(), std::move(role));
        game->addPlayer(std::move(player));
        
        // Update UI
        updatePlayerTable();
        nameField->clear();
        
        // Enable start button if we have at least 2 players
        if (game->players().size() >= 2) {
            startButton->setEnabled(true);
        }
        
        setupStatusLabel->setText(QString("%1 players added. Add more or start game.").arg(game->players().size()));
    }
    
    void updatePlayerTable() {
        // Clear and resize the table
        playerTable->setRowCount(game->players().size());
        
        int row = 0;
        for (const auto& playerName : game->players()) {
            const Player& player = game->getPlayer(playerName);
            
            QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(playerName));
            QTableWidgetItem* roleItem = new QTableWidgetItem(QString::fromStdString(player.getRole()->getName()));
            
            playerTable->setItem(row, 0, nameItem);
            playerTable->setItem(row, 1, roleItem);
            
            row++;
        }
    }
    
    void startGame() {
        if (game->players().size() < 2) {
            QMessageBox::warning(this, "Error", "At least 2 players are required");
            return;
        }

        // Initialize game and reset action tracking
        game->start();
        actionTakenThisTurn = false;
        hasBribed = false;

        populateGameScreen();
        updateGameState();

        // Announce the first player's turn
        Player& firstPlayer = game->getCurrentPlayer();
        QString firstPlayerName = QString::fromStdString(firstPlayer.getName());
        QMessageBox::information(this, "Game Started",
                              QString("Game has started!\n\nIt's %1's turn first.\n\n"
                                    "GAME RULES: Each player can take ONE action per turn.\n"
                                    "Using 'Bribe' allows a second action in the same turn.").arg(firstPlayerName));

        // Set window title to show current player
        setWindowTitle(QString("Coup Game - %1's Turn").arg(firstPlayerName));

        stackedWidget->setCurrentWidget(gameScreen);
    }
    
    void populateGameScreen() {
        // Clear the player stats layout (except headers)
        for (int row = playerStatsLayout->rowCount() - 1; row > 0; row--) {
            for (int col = 0; col < playerStatsLayout->columnCount(); col++) {
                QLayoutItem* item = playerStatsLayout->itemAtPosition(row, col);
                if (item) {
                    if (item->widget()) {
                        delete item->widget();
                    }
                    playerStatsLayout->removeItem(item);
                    delete item;
                }
            }
        }
        
        // Add all players to the stats display
        int row = 1;
        for (const auto& playerName : game->players()) {
            const Player& player = game->getPlayer(playerName);
            
            QLabel* nameLabel = new QLabel(QString::fromStdString(playerName));
            QLabel* roleLabel = new QLabel(QString::fromStdString(player.getRole()->getName()));
            QLabel* coinsLabel = new QLabel(QString::number(player.getCoins()));
            QLabel* statusLabel = new QLabel(player.isActive() ? "Active" : "Eliminated");
            
            // Store the labels for later updates
            playerLabels[playerName] = {nameLabel, roleLabel, coinsLabel, statusLabel};
            
            playerStatsLayout->addWidget(nameLabel, row, 0);
            playerStatsLayout->addWidget(roleLabel, row, 1);
            playerStatsLayout->addWidget(coinsLabel, row, 2);
            playerStatsLayout->addWidget(statusLabel, row, 3);
            
            row++;
        }
        
        // Update target dropdown
        updateTargetCombo();
    }
    
    void updateGameState() {
        if (game->isGameOver()) {
            QString winner = QString::fromStdString(game->winner());
            currentPlayerLabel->setText(QString("Game Over! Winner: %1").arg(winner));
            gameStatusLabel->setText("Game is over");

            // Disable all action buttons
            setActionButtonsEnabled(false);
            return;
        }

        // Update current player info
        Player& currentPlayer = game->getCurrentPlayer();
        QString playerName = QString::fromStdString(currentPlayer.getName());
        QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());

        // Set window title to include current player
        setWindowTitle(QString("Coup Game - %1's Turn").arg(playerName));

        // Update header with current player info and action status
        currentPlayerLabel->setText(QString("Current Player: %1 (%2)").arg(playerName, roleName));

        QString statusText = QString("Coins: %1").arg(currentPlayer.getCoins());
        if (actionTakenThisTurn) {
            if (hasBribed) {
                statusText += " • Used bribe - can take a second action";
            } else {
                statusText += " • Already took an action this turn";
            }
        } else {
            statusText += " • Has not acted this turn";
        }
        gameStatusLabel->setText(statusText);

        // Update player stats with more visible highlighting
        for (const auto& playerName : game->players()) {
            const Player& player = game->getPlayer(playerName);
            if (playerLabels.find(playerName) == playerLabels.end()) {
                // Player not found in the map - this should not happen in normal operation
                continue;
            }
            const auto& labels = playerLabels[playerName];

            // Update stats
            labels.coinsLabel->setText(QString::number(player.getCoins()));
            labels.statusLabel->setText(player.isActive() ? "Active" : "Eliminated");

            // Highlight current player more distinctly
            if (player.getName() == currentPlayer.getName()) {
                QFont boldFont = labels.nameLabel->font();
                boldFont.setBold(true);
                labels.nameLabel->setFont(boldFont);
                labels.roleLabel->setFont(boldFont);
                labels.coinsLabel->setFont(boldFont);
                labels.statusLabel->setFont(boldFont);

                // Use background color for current player
                labels.nameLabel->setStyleSheet("color: white; background-color: blue;");
                labels.roleLabel->setStyleSheet("color: white; background-color: blue;");
                labels.coinsLabel->setStyleSheet("color: white; background-color: blue;");
                labels.statusLabel->setStyleSheet("color: white; background-color: blue;");
            } else {
                QFont normalFont = labels.nameLabel->font();
                normalFont.setBold(false);
                labels.nameLabel->setFont(normalFont);
                labels.roleLabel->setFont(normalFont);
                labels.coinsLabel->setFont(normalFont);
                labels.statusLabel->setFont(normalFont);

                labels.nameLabel->setStyleSheet("");
                labels.roleLabel->setStyleSheet("");
                labels.coinsLabel->setStyleSheet("");
                labels.statusLabel->setStyleSheet("");
            }
        }

        // Update target dropdown and enable/disable appropriate action buttons
        updateTargetCombo();
        updateActionButtons();
    }
    
    void updateTargetCombo() {
        targetCombo->clear();
        
        // Only add active players other than the current player
        Player& currentPlayer = game->getCurrentPlayer();
        for (const auto& playerName : game->players()) {
            if (playerName != currentPlayer.getName()) {
                const Player& player = game->getPlayer(playerName);
                if (player.isActive()) {
                    targetCombo->addItem(QString::fromStdString(playerName));
                }
            }
        }
    }
    
    void updateActionButtons() {
        Player& currentPlayer = game->getCurrentPlayer();
        int coins = currentPlayer.getCoins();
        bool mustCoup = currentPlayer.mustCoup();
        bool isSanctioned = currentPlayer.isSanctioned();
        QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());

        // Check if player has already taken an action (and hasn't bribed)
        bool canTakeAction = !actionTakenThisTurn || hasBribed;

        // Basic actions
        gatherButton->setEnabled(canTakeAction && !mustCoup && !isSanctioned);
        taxButton->setEnabled(canTakeAction && !mustCoup && !isSanctioned);
        bribeButton->setEnabled(canTakeAction && !mustCoup && coins >= 4);
        arrestButton->setEnabled(canTakeAction && !mustCoup && targetCombo->count() > 0);
        sanctionButton->setEnabled(canTakeAction && !mustCoup && coins >= 3 && targetCombo->count() > 0);
        coupButton->setEnabled(canTakeAction && coins >= 7 && targetCombo->count() > 0);

        // Special action
        if (roleName == "Baron" && coins >= 3) {
            specialActionButton->setText("Baron: Invest (costs 3, gain 6)");
            specialActionButton->setEnabled(canTakeAction && !mustCoup);
        } else if (roleName == "Spy" && targetCombo->count() > 0) {
            specialActionButton->setText("Spy: Spy On");
            specialActionButton->setEnabled(canTakeAction && !mustCoup);
        } else if (roleName == "Governor") {
            specialActionButton->setText("Governor: Tax gives 3 coins");
            specialActionButton->setEnabled(false); // Just informational
        } else if (roleName == "General" && targetCombo->count() > 0) {
            specialActionButton->setText("General: Arrest steals all coins");
            specialActionButton->setEnabled(false); // Just informational
        } else if (roleName == "Judge" && targetCombo->count() > 0) {
            specialActionButton->setText("Judge: Sanction blocks all actions");
            specialActionButton->setEnabled(false); // Just informational
        } else if (roleName == "Merchant") {
            specialActionButton->setText("Merchant: Gather gives 2 coins");
            specialActionButton->setEnabled(false); // Just informational
        } else {
            specialActionButton->setText("No special action available");
            specialActionButton->setEnabled(false);
        }

        // If must coup, only allow coup action
        if (mustCoup) {
            setActionButtonsEnabled(false);
            coupButton->setEnabled(canTakeAction); // Can only coup if hasn't already acted
            gameStatusLabel->setText("Player must perform a coup with 10+ coins!");
        }

        // End turn always enabled
        endTurnButton->setEnabled(true);

        // Special case: if player already took action and didn't bribe, can only end turn
        if (actionTakenThisTurn && !hasBribed) {
            setActionButtonsEnabled(false);
            endTurnButton->setEnabled(true);
        }
    }
    
    void setActionButtonsEnabled(bool enabled) {
        gatherButton->setEnabled(enabled);
        taxButton->setEnabled(enabled);
        bribeButton->setEnabled(enabled);
        arrestButton->setEnabled(enabled);
        sanctionButton->setEnabled(enabled);
        coupButton->setEnabled(enabled);
        specialActionButton->setEnabled(enabled);
    }
    
    // Action handlers
    void onGather() {
        try {
            // Check if player already took an action this turn
            if (actionTakenThisTurn && !hasBribed) {
                showMessage("You already took an action this turn. End your turn or use bribe first.");
                return;
            }

            Player& currentPlayer = game->getCurrentPlayer();

            // Before taking the action, check if player must coup or is sanctioned
            if (currentPlayer.mustCoup()) {
                showMessage("Player has 10+ coins and must perform a coup!");
                return;
            }

            if (currentPlayer.isSanctioned()) {
                showMessage("Player is sanctioned and cannot perform economic actions!");
                return;
            }

            // Execute the gather action
            currentPlayer.gather(*game);

            QString message = "Gathered ";
            if (currentPlayer.getRole()->getName() == "Merchant") {
                message += "2 coins (Merchant ability)";
            } else {
                message += "1 coin";
            }

            // Mark that player has taken an action
            if (hasBribed) {
                hasBribed = false;
                message += " (second action after bribe)";
            }
            actionTakenThisTurn = true;

            showMessage(message);
            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::SanctionedException& e) {
            showMessage("You are sanctioned and cannot perform economic actions!");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }

    void onTax() {
        try {
            Player& currentPlayer = game->getCurrentPlayer();

            // Before taking the action, check if player must coup or is sanctioned
            if (currentPlayer.mustCoup()) {
                showMessage("Player has 10+ coins and must perform a coup!");
                return;
            }

            if (currentPlayer.isSanctioned()) {
                showMessage("Player is sanctioned and cannot perform economic actions!");
                return;
            }

            // Execute the tax action
            currentPlayer.tax(*game);

            QString message = "Taxed ";
            if (currentPlayer.getRole()->getName() == "Governor") {
                message += "3 coins (Governor ability)";
            } else {
                message += "2 coins";
            }

            showMessage(message);
            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::SanctionedException& e) {
            showMessage("You are sanctioned and cannot perform economic actions!");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }
    
    void onBribe() {
        try {
            // Check if player already took an action this turn
            if (actionTakenThisTurn && !hasBribed) {
                showMessage("You already took an action this turn. End your turn.");
                return;
            }

            Player& currentPlayer = game->getCurrentPlayer();

            // Check if player must coup
            if (currentPlayer.mustCoup()) {
                showMessage("Player has 10+ coins and must perform a coup!");
                return;
            }

            // Check if player has enough coins
            if (currentPlayer.getCoins() < 4) {
                showMessage("Not enough coins to bribe. Requires 4 coins.");
                return;
            }

            currentPlayer.bribe(*game);

            // Set the bribe flag to allow a second action
            if (!actionTakenThisTurn) {
                actionTakenThisTurn = true;
                hasBribed = true;
                showMessage("Bribed officials for 4 coins. You can take one more action this turn.");
            } else {
                // This was their second action after a previous bribe
                actionTakenThisTurn = true;
                hasBribed = false;
                showMessage("Bribed officials for 4 coins (second action after bribe).");
            }

            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::InsufficientCoinsException& e) {
            showMessage("Not enough coins to bribe. Requires 4 coins.");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }

    void onArrest() {
        if (targetCombo->currentText().isEmpty()) {
            showMessage("Please select a target player");
            return;
        }

        try {
            Player& currentPlayer = game->getCurrentPlayer();
            std::string targetName = targetCombo->currentText().toStdString();
            Player& targetPlayer = game->getPlayer(targetName);

            // Check if player must coup
            if (currentPlayer.mustCoup()) {
                showMessage("Player has 10+ coins and must perform a coup!");
                return;
            }

            // Check if targeting the same player as last turn
            if (targetName == currentPlayer.getLastArrestedPlayer()) {
                showMessage("Cannot arrest the same player in consecutive turns.");
                return;
            }

            currentPlayer.arrest(targetPlayer, *game);

            QString message = "Arrested " + targetCombo->currentText();
            if (currentPlayer.getRole()->getName() == "General") {
                message += ", stealing all coins (General ability)";
            } else {
                message += ", stealing 1 coin";
            }

            showMessage(message);
            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::ConsecutiveArrestException& e) {
            showMessage("Cannot arrest the same player in consecutive turns.");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }

    void onSanction() {
        if (targetCombo->currentText().isEmpty()) {
            showMessage("Please select a target player");
            return;
        }

        try {
            Player& currentPlayer = game->getCurrentPlayer();
            std::string targetName = targetCombo->currentText().toStdString();
            Player& targetPlayer = game->getPlayer(targetName);

            // Check if player must coup
            if (currentPlayer.mustCoup()) {
                showMessage("Player has 10+ coins and must perform a coup!");
                return;
            }

            // Check if player has enough coins
            if (currentPlayer.getCoins() < 3) {
                showMessage("Not enough coins to sanction. Requires 3 coins.");
                return;
            }

            currentPlayer.sanction(targetPlayer, *game);

            QString message = "Sanctioned " + targetCombo->currentText() + " (costs 3 coins)";
            if (currentPlayer.getRole()->getName() == "Judge") {
                message += ", blocking all actions (Judge ability)";
            } else {
                message += ", blocking economic actions";
            }

            showMessage(message);
            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::InsufficientCoinsException& e) {
            showMessage("Not enough coins to sanction. Requires 3 coins.");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }

    void onCoup() {
        if (targetCombo->currentText().isEmpty()) {
            showMessage("Please select a target player");
            return;
        }

        try {
            Player& currentPlayer = game->getCurrentPlayer();
            std::string targetName = targetCombo->currentText().toStdString();
            Player& targetPlayer = game->getPlayer(targetName);

            // Check if player has enough coins
            if (currentPlayer.getCoins() < 7) {
                showMessage("Not enough coins to coup. Requires 7 coins.");
                return;
            }

            currentPlayer.coup(targetPlayer, *game);

            showMessage("Performed coup on " + targetCombo->currentText() + " (costs 7 coins)");
            updateGameState();

            // Check if game is over
            if (game->isGameOver()) {
                QString winner = QString::fromStdString(game->winner());
                QMessageBox::information(this, "Game Over",
                                       "Game over! " + winner + " wins!");
            }
        } catch (const Coup::InsufficientCoinsException& e) {
            showMessage("Not enough coins to coup. Requires 7 coins.");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }
    
    void onSpecialAction() {
        Player& currentPlayer = game->getCurrentPlayer();
        QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());

        try {
            // Baron's special ability
            if (roleName == "Baron") {
                // Check if player has enough coins
                if (currentPlayer.getCoins() < 3) {
                    showMessage("Baron needs at least 3 coins to invest");
                    return;
                }

                // Check if player must coup
                if (currentPlayer.mustCoup()) {
                    showMessage("Player has 10+ coins and must perform a coup!");
                    return;
                }

                Baron* baronRole = dynamic_cast<Baron*>(currentPlayer.getRole());
                baronRole->invest(currentPlayer, *game);
                showMessage("Baron invested 3 coins to receive 6 coins");
            }
            // Spy's special ability
            else if (roleName == "Spy") {
                if (targetCombo->currentText().isEmpty()) {
                    showMessage("Please select a target player to spy on");
                    return;
                }

                // Check if player must coup
                if (currentPlayer.mustCoup()) {
                    showMessage("Player has 10+ coins and must perform a coup!");
                    return;
                }

                std::string targetName = targetCombo->currentText().toStdString();
                Player& targetPlayer = game->getPlayer(targetName);

                Spy* spyRole = dynamic_cast<Spy*>(currentPlayer.getRole());
                spyRole->spyOn(targetPlayer, *game);

                showMessage("Spied on " + targetCombo->currentText() + ": They have " +
                            QString::number(targetPlayer.getCoins()) + " coins");
            }

            updateGameState();
        } catch (const Coup::MandatoryCoupException& e) {
            showMessage("You must perform a coup with 10+ coins!");
        } catch (const Coup::InsufficientCoinsException& e) {
            showMessage("Not enough coins for this action.");
        } catch (const std::exception& e) {
            showMessage(QString("Error: %1").arg(e.what()));
        }
    }

    void onEndTurn() {
        // Before ending the turn, check if there's any unfinished mandatory action
        Player& currentPlayer = game->getCurrentPlayer();
        QString oldPlayerName = QString::fromStdString(currentPlayer.getName());

        // If player has 10+ coins, they must coup before ending turn
        if (currentPlayer.mustCoup()) {
            QMessageBox::warning(this, "Cannot End Turn", "You must perform a coup with 10+ coins before ending your turn!");
            return;
        }

        // Save the current player name first
        QString currentName = QString::fromStdString(currentPlayer.getName());

        // Reset action tracking for the next player's turn
        actionTakenThisTurn = false;
        hasBribed = false;

        // Advance to next player
        game->nextTurn();

        // Get the new current player name
        Player& newCurrentPlayer = game->getCurrentPlayer();
        QString newPlayerName = QString::fromStdString(newCurrentPlayer.getName());

        // Flash the window title to indicate turn change
        setWindowTitle("TURN CHANGING...");

        // Update the UI
        updateGameState();

        // If game is over after turn change, handle it
        if (game->isGameOver()) {
            QString winner = QString::fromStdString(game->winner());
            QMessageBox::information(this, "Game Over", "Game over! " + winner + " wins!");
        } else {
            // Make turn change very obvious with a clear message box
            QMessageBox::information(this, "Turn Changed",
                QString("Turn changed from %1 to %2").arg(oldPlayerName, newPlayerName));

            // Ensure the window title reflects the new player
            setWindowTitle(QString("Coup Game - %1's Turn").arg(newPlayerName));
        }
    }
    
    void showMessage(const QString &message) {
        gameStatusLabel->setText(message);
        QMessageBox::information(this, "Coup Game", message);
    }
    
    // Setup screen widgets
    QWidget* setupScreen;
    QLineEdit* nameField;
    QComboBox* roleCombo;
    QTableWidget* playerTable;
    QPushButton* startButton;
    QLabel* setupStatusLabel;
    
    // Game screen widgets
    QWidget* gameScreen;
    QLabel* currentPlayerLabel;
    QLabel* gameStatusLabel;
    QGridLayout* playerStatsLayout;
    QComboBox* targetCombo;
    QGroupBox* actionBox;
    QGroupBox* basicActionsBox;
    QPushButton* gatherButton;
    QPushButton* taxButton;
    QPushButton* bribeButton;
    QPushButton* arrestButton;
    QPushButton* sanctionButton;
    QPushButton* coupButton;
    QPushButton* specialActionButton;
    QPushButton* endTurnButton;
    
    // Main UI components
    QStackedWidget* stackedWidget;
    
    // Game state
    std::unique_ptr<Game> game;
    bool actionTakenThisTurn;  // Track if the player has taken their action this turn
    bool hasBribed;            // Track if player has bribed this turn (allows a second action)

    // Structure to store player display labels for easy updating
    struct PlayerLabels {
        QLabel* nameLabel;
        QLabel* roleLabel;
        QLabel* coinsLabel;
        QLabel* statusLabel;
    };

    std::unordered_map<std::string, PlayerLabels> playerLabels;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    CoupGUI mainWindow;
    mainWindow.show();
    
    return app.exec();
}