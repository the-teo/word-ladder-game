#pragma once
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QFile>
#include <QTextStream>

/**
 * @class GameSession
 * @brief Represents a single play session in the Word Ladder game.
 * @ingroup Core Core Logic
 *
 * Stores details such as the player name, start and target words, moves made,
 * hints used, and allows saving/loading session data to/from a file.
 */

class GameSession {
private:
    QString playerName;        ///< The name of the player.
    QDateTime startTime;       ///< The timestamp when the game started.
    QString startWord;         ///< The starting word of the session.
    QString targetWord;        ///< The target word to reach.
    QStringList moves;         ///< List of words guessed by the player.
    int hintsUsed;             ///< Number of hints used during the session.
    int optimalMoves;          ///< Optimal number of moves between start and target.

public:
    /**
     * @brief Constructs a new GameSession object.
     *
     * @param name Name of the player.
     * @param start Starting word.
     * @param target Target word.
     * @param optimal Optimal number of moves to reach the target.
     */
    GameSession(const QString& name, const QString& start, const QString& target, int optimal);

    /**
     * @brief Adds a new move (word guess) to the session.
     *
     * @param word The guessed word to be added.
     */
    void addMove(const QString& word);

    /**
     * @brief Increments the count of hints used.
     */
    void incrementHints();

    /**
     * @brief Saves the current session data to a file.
     *
     * Each player's sessions are stored in a text file named after the player.
     */
    void saveToFile() const;

    /**
     * @brief Loads all sessions associated with a specific user.
     *
     * @param username The player name whose sessions are to be loaded.
     * @return A list of GameSession objects.
     */
    static QList<GameSession> loadFromFile(const QString& username);

    // Getters

    /**
     * @brief Returns the name of the player.
     * @return Player name.
     */
    QString getPlayerName() const;

    /**
     * @brief Returns the start time of the session.
     * @return QDateTime when the session began.
     */
    QDateTime getStartTime() const;

    /**
     * @brief Returns the starting word of the session.
     * @return Starting word.
     */
    QString getStartWord() const;

    /**
     * @brief Returns the target word of the session.
     * @return Target word.
     */
    QString getTargetWord() const;

    /**
     * @brief Returns the list of moves (word guesses).
     * @return List of guessed words.
     */
    QStringList getMoves() const;

    /**
     * @brief Returns the number of hints used.
     * @return Hint count.
     */
    int getHintsUsed() const;

    /**
     * @brief Returns the number of moves made.
     * @return Move count.
     */
    int getMoveCount() const;

    /**
     * @brief Returns the optimal number of moves between start and target.
     * @return Optimal move count.
     */
    int getOptimalMoves() const;

    /**
     * @brief Returns the last word guessed (current word).
     * @return Current word in the ladder.
     */
    QString getCurrentWord() const;

    /**
     * @brief Checks if the game is complete.
     *
     * Completion is defined as the last guessed word being equal to the target word.
     *
     * @return True if the session is complete, false otherwise.
     */
    bool isComplete() const;
};
