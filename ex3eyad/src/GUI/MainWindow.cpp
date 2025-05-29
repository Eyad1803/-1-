#include "../../include/GUI/MainWindow.hpp"
#include "../../include/Game.hpp"
#include "../../include/Player.hpp"
#include "../../include/Role.hpp"
#include "../../include/Roles/Governor.hpp"
#include "../../include/Roles/Spy.hpp"
#include "../../include/Roles/Baron.hpp"
#include "../../include/Roles/General.hpp"
#include "../../include/Roles/Judge.hpp"
#include "../../include/Roles/Merchant.hpp"
#include "../../include/Exceptions.hpp"

#include <QMessageBox>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QStyleOption>
#include <QLabel>
#include <QFrame>
#include <QDialogButtonBox>
#include <QStackedLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameInProgress(false)
{
    setupUI();
    setWindowTitle("Coup Game");
    resize(800, 600);
}

MainWindow::~MainWindow() {
    // Resources will be cleaned up automatically
}

void MainWindow::setupUI() {
    // Create a central widget with a stacked layout
    QWidget *centralWidget = new QWidget(this);
    QStackedLayout *stackedLayout = new QStackedLayout(centralWidget);
    setCentralWidget(centralWidget);
    
    // Setup widget (for game setup)
    setupWidget = new QWidget();
    QVBoxLayout *setupLayout = new QVBoxLayout(setupWidget);
    
    // Title
    QLabel *titleLabel = new QLabel("Coup Game Setup");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    setupLayout->addWidget(titleLabel);
    
    // Add player section
    QGroupBox *addPlayerGroup = new QGroupBox("Add Player");
    QHBoxLayout *addPlayerLayout = new QHBoxLayout(addPlayerGroup);
    
    playerNameEdit = new QLineEdit();
    playerNameEdit->setPlaceholderText("Player Name");
    addPlayerLayout->addWidget(playerNameEdit);
    
    roleComboBox = new QComboBox();
    roleComboBox->addItem("Governor");
    roleComboBox->addItem("Spy");
    roleComboBox->addItem("Baron");
    roleComboBox->addItem("General");
    roleComboBox->addItem("Judge");
    roleComboBox->addItem("Merchant");
    addPlayerLayout->addWidget(roleComboBox);
    
    addPlayerButton = new QPushButton("Add Player");
    addPlayerLayout->addWidget(addPlayerButton);
    
    setupLayout->addWidget(addPlayerGroup);
    
    // Players list
    QGroupBox *playersGroup = new QGroupBox("Players");
    QVBoxLayout *playersLayout = new QVBoxLayout(playersGroup);
    
    playersListLayout = new QVBoxLayout();
    playersLayout->addLayout(playersListLayout);
    
    setupLayout->addWidget(playersGroup);
    
    // Start game button
    startGameButton = new QPushButton("Start Game");
    startGameButton->setEnabled(false);  // Disable until at least 2 players
    setupLayout->addWidget(startGameButton);
    
    // Status label
    setupStatusLabel = new QLabel("Add 2-6 players to start the game.");
    setupStatusLabel->setAlignment(Qt::AlignCenter);
    setupLayout->addWidget(setupStatusLabel);
    
    // Game widget (for actual gameplay)
    gameWidget = new QWidget();
    QVBoxLayout *gameLayout = new QVBoxLayout(gameWidget);
    
    // Current player info
    QGroupBox *playerInfoGroup = new QGroupBox("Current Player");
    QVBoxLayout *playerInfoLayout = new QVBoxLayout(playerInfoGroup);
    
    currentPlayerLabel = new QLabel("Player: ");
    currentRoleLabel = new QLabel("Role: ");
    coinsLabel = new QLabel("Coins: 0");
    
    playerInfoLayout->addWidget(currentPlayerLabel);
    playerInfoLayout->addWidget(currentRoleLabel);
    playerInfoLayout->addWidget(coinsLabel);
    
    gameLayout->addWidget(playerInfoGroup);
    
    // Actions
    actionsGroup = new QGroupBox("Actions");
    actionsLayout = new QVBoxLayout(actionsGroup);
    
    gatherButton = new QPushButton("Gather (1 coin)");
    taxButton = new QPushButton("Tax (2-3 coins)");
    bribeButton = new QPushButton("Bribe (pay 4 coins)");
    arrestButton = new QPushButton("Arrest (steal 1 coin)");
    sanctionButton = new QPushButton("Sanction (costs 3 coins)");
    coupButton = new QPushButton("Coup (costs 7 coins)");
    specialActionButton = new QPushButton("Special Action");
    endTurnButton = new QPushButton("End Turn");
    
    actionsLayout->addWidget(gatherButton);
    actionsLayout->addWidget(taxButton);
    actionsLayout->addWidget(bribeButton);
    actionsLayout->addWidget(arrestButton);
    actionsLayout->addWidget(sanctionButton);
    actionsLayout->addWidget(coupButton);
    actionsLayout->addWidget(specialActionButton);
    
    // Target selection
    QHBoxLayout *targetLayout = new QHBoxLayout();
    QLabel *targetLabel = new QLabel("Target:");
    targetComboBox = new QComboBox();
    
    targetLayout->addWidget(targetLabel);
    targetLayout->addWidget(targetComboBox);
    actionsLayout->addLayout(targetLayout);
    
    actionsLayout->addWidget(endTurnButton);
    
    gameLayout->addWidget(actionsGroup);
    
    // Players display
    QGroupBox *playersDisplayGroup = new QGroupBox("All Players");
    QVBoxLayout *playersDisplayLayout = new QVBoxLayout(playersDisplayGroup);
    
    playersGridLayout = new QGridLayout();
    playersDisplayLayout->addLayout(playersGridLayout);
    
    gameLayout->addWidget(playersDisplayGroup);
    
    // Game status
    gameStatusLabel = new QLabel("Game in progress");
    gameStatusLabel->setAlignment(Qt::AlignCenter);
    gameLayout->addWidget(gameStatusLabel);
    
    // Add widgets to stacked layout
    stackedLayout->addWidget(setupWidget);
    stackedLayout->addWidget(gameWidget);
    
    // Connect signals and slots
    connect(addPlayerButton, &QPushButton::clicked, this, &MainWindow::onAddPlayer);
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartGame);
    
    connect(gatherButton, &QPushButton::clicked, this, &MainWindow::onGather);
    connect(taxButton, &QPushButton::clicked, this, &MainWindow::onTax);
    connect(bribeButton, &QPushButton::clicked, this, &MainWindow::onBribe);
    connect(arrestButton, &QPushButton::clicked, this, &MainWindow::onArrest);
    connect(sanctionButton, &QPushButton::clicked, this, &MainWindow::onSanction);
    connect(coupButton, &QPushButton::clicked, this, &MainWindow::onCoup);
    connect(specialActionButton, &QPushButton::clicked, this, &MainWindow::onSpecialAction);
    connect(endTurnButton, &QPushButton::clicked, this, &MainWindow::onEndTurn);
    
    // Show setup screen initially
    stackedLayout->setCurrentWidget(setupWidget);
}

void MainWindow::onAddPlayer() {
    QString name = playerNameEdit->text().trimmed();
    if (name.isEmpty()) {
        showMessage("Please enter a player name");
        return;
    }
    
    // Create a role based on selection
    std::unique_ptr<Role> role;
    QString roleName = roleComboBox->currentText();
    
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
    
    // Create player
    if (!game) {
        game = std::make_unique<Game>();
    }
    
    try {
        // Add player to game
        std::unique_ptr<Player> player = std::make_unique<Player>(name.toStdString(), std::move(role));
        game->addPlayer(std::move(player));
        
        // Update UI
        updatePlayersList();
        playerNameEdit->clear();
        
        // Enable start button if we have at least 2 players
        if (game->players().size() >= 2) {
            startGameButton->setEnabled(true);
        }
        
        setupStatusLabel->setText(QString("%1 players added. Add more or start the game.").arg(game->players().size()));
    } catch (const std::exception& e) {
        showMessage(QString("Error adding player: %1").arg(e.what()));
    }
}

void MainWindow::updatePlayersList() {
    // Clear the current list
    QLayoutItem *child;
    while ((child = playersListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }
    
    // Add all players
    for (const auto& playerName : game->players()) {
        const Player& player = game->getPlayer(playerName);
        QString roleText = QString::fromStdString(player.getRole()->getName());
        
        QLabel *playerLabel = new QLabel(QString("%1 (%2)").arg(
            QString::fromStdString(playerName), 
            roleText
        ));
        playersListLayout->addWidget(playerLabel);
    }
}

void MainWindow::onStartGame() {
    if (game->players().size() < 2) {
        showMessage("At least 2 players are required to start the game");
        return;
    }
    
    // Start the game
    game->start();
    gameInProgress = true;
    
    // Switch to game screen
    QStackedLayout *stackedLayout = qobject_cast<QStackedLayout*>(centralWidget()->layout());
    if (stackedLayout) {
        stackedLayout->setCurrentWidget(gameWidget);
    }
    
    // Setup game UI
    setupGameUI();
}

void MainWindow::setupGameUI() {
    // Clear player display area
    playerLabels.clear();
    
    // Setup player display
    int row = 0;
    for (const auto& playerName : game->players()) {
        const Player& player = game->getPlayer(playerName);
        
        QLabel *nameLabel = new QLabel(QString::fromStdString(playerName));
        QLabel *roleLabel = new QLabel(QString::fromStdString(player.getRole()->getName()));
        QLabel *coinsLabel = new QLabel(QString::number(player.getCoins()));
        QLabel *statusLabel = new QLabel(player.isActive() ? "Active" : "Eliminated");
        
        playersGridLayout->addWidget(nameLabel, row, 0);
        playersGridLayout->addWidget(roleLabel, row, 1);
        playersGridLayout->addWidget(coinsLabel, row, 2);
        playersGridLayout->addWidget(statusLabel, row, 3);
        
        playerLabels.push_back(coinsLabel);
        playerLabels.push_back(statusLabel);
        
        row++;
    }
    
    // Update game state display
    updateGameState();
}

void MainWindow::updateGameState() {
    if (!gameInProgress || game->isGameOver()) {
        // Game over - show winner
        if (game->isGameOver()) {
            QString winner = QString::fromStdString(game->winner());
            gameStatusLabel->setText(QString("Game Over! Winner: %1").arg(winner));
            
            // Disable all action buttons
            gatherButton->setEnabled(false);
            taxButton->setEnabled(false);
            bribeButton->setEnabled(false);
            arrestButton->setEnabled(false);
            sanctionButton->setEnabled(false);
            coupButton->setEnabled(false);
            specialActionButton->setEnabled(false);
            endTurnButton->setEnabled(false);
        }
        return;
    }
    
    // Update current player info
    Player& currentPlayer = game->getCurrentPlayer();
    currentPlayerLabel->setText(QString("Player: %1").arg(QString::fromStdString(currentPlayer.getName())));
    currentRoleLabel->setText(QString("Role: %1").arg(QString::fromStdString(currentPlayer.getRole()->getName())));
    coinsLabel->setText(QString("Coins: %1").arg(currentPlayer.getCoins()));
    
    // Update target combo box
    targetComboBox->clear();
    for (const auto& playerName : game->players()) {
        if (playerName != currentPlayer.getName()) {
            targetComboBox->addItem(QString::fromStdString(playerName));
        }
    }
    
    // Update player display area
    int index = 0;
    for (const auto& playerName : game->players()) {
        const Player& player = game->getPlayer(playerName);
        
        // Update coins and status
        QLabel *coinsLabel = playerLabels[index * 2];
        QLabel *statusLabel = playerLabels[index * 2 + 1];
        
        coinsLabel->setText(QString::number(player.getCoins()));
        statusLabel->setText(player.isActive() ? "Active" : "Eliminated");
        
        index++;
    }
    
    // Update available actions
    updatePlayerActions();
}

void MainWindow::updatePlayerActions() {
    Player& currentPlayer = game->getCurrentPlayer();
    int coins = currentPlayer.getCoins();
    bool mustCoup = currentPlayer.mustCoup();
    
    // Basic actions
    gatherButton->setEnabled(!mustCoup && !currentPlayer.isSanctioned());
    taxButton->setEnabled(!mustCoup && !currentPlayer.isSanctioned());
    bribeButton->setEnabled(!mustCoup && coins >= 4);
    arrestButton->setEnabled(!mustCoup);
    sanctionButton->setEnabled(!mustCoup && coins >= 3);
    coupButton->setEnabled(coins >= 7);
    
    // Special action based on role
    QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());
    
    if (roleName == "Baron" && coins >= 3) {
        specialActionButton->setText("Baron: Invest");
        specialActionButton->setEnabled(true);
    } else if (roleName == "Spy") {
        specialActionButton->setText("Spy: Spy On");
        specialActionButton->setEnabled(true);
    } else {
        specialActionButton->setText("Special Action");
        specialActionButton->setEnabled(false);
    }
    
    // If must coup, disable all other actions
    if (mustCoup) {
        gatherButton->setEnabled(false);
        taxButton->setEnabled(false);
        bribeButton->setEnabled(false);
        arrestButton->setEnabled(false);
        sanctionButton->setEnabled(false);
        specialActionButton->setEnabled(false);
        
        gameStatusLabel->setText("Player must perform a coup with 10+ coins");
    } else {
        gameStatusLabel->setText("Game in progress");
    }
}

void MainWindow::onGather() {
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        currentPlayer.gather(*game);
        
        showMessage("Gathered 1 coin");
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onTax() {
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        currentPlayer.tax(*game);
        
        QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());
        if (roleName == "Governor") {
            showMessage("Taxed 3 coins (Governor ability)");
        } else {
            showMessage("Taxed 2 coins");
        }
        
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onBribe() {
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        currentPlayer.bribe(*game);
        
        showMessage("Bribed for an extra action (costs 4 coins)");
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onArrest() {
    if (targetComboBox->currentText().isEmpty()) {
        showMessage("Please select a target player");
        return;
    }
    
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        std::string targetName = targetComboBox->currentText().toStdString();
        Player& targetPlayer = game->getPlayer(targetName);
        
        currentPlayer.arrest(targetPlayer, *game);
        
        showMessage(QString("Arrested %1, stealing 1 coin").arg(targetComboBox->currentText()));
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onSanction() {
    if (targetComboBox->currentText().isEmpty()) {
        showMessage("Please select a target player");
        return;
    }
    
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        std::string targetName = targetComboBox->currentText().toStdString();
        Player& targetPlayer = game->getPlayer(targetName);
        
        currentPlayer.sanction(targetPlayer, *game);
        
        showMessage(QString("Sanctioned %1 (costs 3 coins)").arg(targetComboBox->currentText()));
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onCoup() {
    if (targetComboBox->currentText().isEmpty()) {
        showMessage("Please select a target player");
        return;
    }
    
    try {
        Player& currentPlayer = game->getCurrentPlayer();
        std::string targetName = targetComboBox->currentText().toStdString();
        Player& targetPlayer = game->getPlayer(targetName);
        
        currentPlayer.coup(targetPlayer, *game);
        
        showMessage(QString("Performed coup on %1 (costs 7 coins)").arg(targetComboBox->currentText()));
        updateGameState();
        
        // Check if game is over
        if (game->isGameOver()) {
            QMessageBox::information(this, "Game Over", 
                                   QString("Game over! %1 wins!").arg(
                                       QString::fromStdString(game->winner())));
        }
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onSpecialAction() {
    Player& currentPlayer = game->getCurrentPlayer();
    QString roleName = QString::fromStdString(currentPlayer.getRole()->getName());
    
    try {
        if (roleName == "Baron") {
            Baron* baronRole = dynamic_cast<Baron*>(currentPlayer.getRole());
            baronRole->invest(currentPlayer, *game);
            showMessage("Baron invested 3 coins to receive 6");
        } else if (roleName == "Spy") {
            if (targetComboBox->currentText().isEmpty()) {
                showMessage("Please select a target player");
                return;
            }
            
            std::string targetName = targetComboBox->currentText().toStdString();
            Player& targetPlayer = game->getPlayer(targetName);
            
            Spy* spyRole = dynamic_cast<Spy*>(currentPlayer.getRole());
            spyRole->spyOn(targetPlayer, *game);
            
            showMessage(QString("Spied on %1: They have %2 coins")
                        .arg(targetComboBox->currentText())
                        .arg(targetPlayer.getCoins()));
        }
        
        updateGameState();
    } catch (const std::exception& e) {
        showMessage(QString("Error: %1").arg(e.what()));
    }
}

void MainWindow::onEndTurn() {
    game->nextTurn();
    updateGameState();
    
    showMessage(QString("Now %1's turn").arg(QString::fromStdString(game->turn())));
}

void MainWindow::showMessage(const QString &message) {
    gameStatusLabel->setText(message);
    QMessageBox::information(this, "Coup Game", message);
}