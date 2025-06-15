#include "GameSession.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStringList>


 GameSession::GameSession(const QString& name, const QString& start, const QString& target, int optimal)
    : playerName(name), startTime(QDateTime::currentDateTime()),
    startWord(start), targetWord(target), hintsUsed(0), optimalMoves(optimal) {
    moves.append(startWord);
}

 void GameSession::addMove(const QString& word) { moves.append(word); }

 void GameSession::incrementHints() { hintsUsed++; }

 void GameSession::saveToFile() const {
    QString filename = playerName.toLower().replace(" ", "_") + ".csv";
    QFile file(filename);
    bool exists = file.exists();

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);

        // Write header if file is new
        if (!exists) {
            out << "Timestamp,Player,StartWord,TargetWord,Moves,HintsUsed,UserMoves,OptimalMoves\n";
        }

        // Write data
        out << startTime.toString(Qt::ISODate) << ","
            << playerName << ","
            << startWord << ","
            << targetWord << ","
            << moves.join("->") << ","
            << hintsUsed << ","
            << (moves.isEmpty() ? 0 : moves.size() - 1) << ","
            << optimalMoves << "\n";
    }
}

 QList<GameSession> GameSession::loadFromFile(const QString& username) {
    QList<GameSession> sessions;
    QString filename = username.toLower().replace(" ", "_") + ".csv";
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // Skip header
        if (!in.atEnd()) in.readLine();

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(',');
            if (parts.size() >= 8) {
                GameSession session(parts[1], parts[2], parts[3], parts[7].toInt());
                session.startTime = QDateTime::fromString(parts[0], Qt::ISODate);
                session.moves = parts[4].split("->");
                session.hintsUsed = parts[5].toInt();
                sessions.append(session);
            }
        }
    }
    return sessions;
}

// Getters
 QString GameSession::getPlayerName() const { return playerName; }

 QDateTime GameSession::getStartTime() const { return startTime; }

 QString GameSession::getStartWord() const { return startWord; }

 QString GameSession::getTargetWord() const { return targetWord; }

 QStringList GameSession::getMoves() const { return moves; }

 int GameSession::getHintsUsed() const { return hintsUsed; }

 int GameSession::getMoveCount() const { return moves.size() - 1; }

 int GameSession::getOptimalMoves() const { return optimalMoves; }

 QString GameSession::getCurrentWord() const { return moves.isEmpty() ? "" : moves.last(); }

 bool GameSession::isComplete() const { return !moves.isEmpty() && moves.last() == targetWord; }
