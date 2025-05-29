#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QStackedWidget>
#include <QLineEdit>
#include <vector>
#include <memory>

// Forward declarations
class Game;
class Player;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Setup slots
    void onAddPlayer();
    void onStartGame();
    
    // Game action slots
    void onGather();
    void onTax();
    void onBribe();
    void onArrest();
    void onSanction();
    void onCoup();
    void onSpecialAction(); // For role-specific actions
    void onEndTurn();
    
private:
    // Game state
    std::unique_ptr<Game> game;
    bool gameInProgress;
    
    // Setup UI components
    QWidget *setupWidget;
    QLineEdit *playerNameEdit;
    QComboBox *roleComboBox;
    QPushButton *addPlayerButton;
    QPushButton *startGameButton;
    QLabel *setupStatusLabel;
    QVBoxLayout *playersListLayout;
    
    // Game UI components
    QWidget *gameWidget;
    QLabel *currentPlayerLabel;
    QLabel *currentRoleLabel;
    QLabel *coinsLabel;
    QGroupBox *actionsGroup;
    QVBoxLayout *actionsLayout;
    QPushButton *gatherButton;
    QPushButton *taxButton;
    QPushButton *bribeButton;
    QPushButton *arrestButton;
    QPushButton *sanctionButton;
    QPushButton *coupButton;
    QPushButton *specialActionButton;
    QPushButton *endTurnButton;
    QComboBox *targetComboBox;
    QLabel *gameStatusLabel;
    
    // Player display area
    QGridLayout *playersGridLayout;
    std::vector<QLabel*> playerLabels;
    
    // Methods
    void setupUI();
    void setupGameUI();
    void updatePlayersList();
    void updateGameState();
    void updatePlayerActions();
    void showMessage(const QString &message);
};