#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include "GameSession.h"
#include "Solver.h"
#include "GraphBuilder.h"

/**
 * @class MainWindow
 * @brief The main window of the Word Ladder Qt application.
 * @ingroup UI UI Components
 *
 * This class manages the UI and logic for the three main functionalities:
 * - Automatic mode for solving word ladders
 * - Interactive play mode
 * - Analytics viewing mode
 */

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs the main application window.
     * @param parent Pointer to the parent widget.
     */
    MainWindow(QWidget* parent = nullptr);

    /**
     * @brief Destroys the main application window.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Handles the Solve button click in automatic mode.
     */
    void onSolveClicked();

    /**
     * @brief Handles the Start Playing button click in play mode.
     */
    void onStartPlayingClicked();

    /**
     * @brief Handles changes in the word length combo box.
     * @param index Index of the selected word length.
     */
    void onWordLengthChanged(int index);

    /**
     * @brief Handles the Make Move button click in play mode.
     */
    void onMakeMoveClicked();

    /**
     * @brief Handles the Hint button click in play mode.
     */
    void onHintClicked();

    /**
     * @brief Handles the Give Up button click in play mode.
     */
    void onGiveUpClicked();

    /**
     * @brief Handles the Load Analytics button click in analytics mode.
     */
    void onLoadAnalyticsClicked();

private:
    /**
     * @brief Sets up the automatic mode tab.
     */
    void setupAutomaticTab();

    /**
     * @brief Sets up the play mode tab.
     */
    void setupPlayTab();

    /**
     * @brief Sets up the analytics tab.
     */
    void setupAnalyticsTab();

    /**
     * @brief Loads the dictionary file into memory and builds the word graph.
     */
    void loadDictionary();

    /**
     * @brief Updates the UI elements that display the game state.
     */
    void updateGameDisplay();

    /**
     * @brief Ends the current game session and resets related UI components.
     */
    void endGame();

    /**
     * @brief Enables or disables controls related to gameplay.
     * @param enabled Whether controls should be enabled or disabled.
     */
    void setGameControlsEnabled(bool enabled);

    // UI Elements
    QWidget* automaticTab;       ///< Tab for automatic solver
    QWidget* playTab;            ///< Tab for interactive play mode
    QWidget* analyticsTab;       ///< Tab for viewing analytics

    // Automatic Tab
    QComboBox* wordLengthComboAuto;   ///< Combo box for word length selection
    QComboBox* startWordComboAuto;    ///< Combo box for start word selection
    QComboBox* targetWordComboAuto;   ///< Combo box for target word selection
    QPushButton* solveButton;         ///< Button to trigger solving
    QListWidget* solutionList;        ///< List widget to show solution steps

    // Play Tab
    QLineEdit* playerNameEdit;        ///< Input for player name
    QComboBox* wordLengthComboPlay;   ///< Combo box for word length in play mode
    QLabel* currentWordLabel;         ///< Label showing the current word
    QLabel* targetWordLabel;          ///< Label showing the target word
    QLineEdit* wordInput;             ///< Input field for player's next word
    QPushButton* makeMoveButton;      ///< Button to submit a move
    QPushButton* hintButton;          ///< Button to request a hint
    QPushButton* giveUpButton;        ///< Button to give up the game
    QTextEdit* gameLog;               ///< Log showing game progress
    QLabel* hintLabel;                ///< Label showing a hint
    QLabel* statsLabel;               ///< Label showing player stats

    // Analytics Tab
    QLineEdit* analyticsNameEdit;     ///< Input for player's name to load analytics
    QPushButton* loadAnalyticsButton; ///< Button to load analytics
    QTextEdit* analyticsOutput;       ///< Text area showing analytics results

    // Game Data
    GameSession* currentGame;         ///< Current game session object
    Solver* solver;                   ///< Solver object for automatic mode
    Graph<std::string> wordGraph;     ///< Graph used for solving word ladders
    std::vector<std::string> dictionary; ///< Loaded dictionary of valid words
    QString currentDictionaryFile = "dictionary.txt"; ///< Path to dictionary file
};
