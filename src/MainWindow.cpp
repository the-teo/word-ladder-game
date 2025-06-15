#include "MainWindow.h"
#include "GraphBuilder.h"
#include "Solver.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QRegularExpression>
#include <QPalette>
#include <QRandomGenerator>
#include <random>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), currentGame(nullptr), solver(nullptr) {
    setWindowTitle("Word Ladder Game");
    resize(800, 600);

    QTabWidget* tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    setupAutomaticTab();
    setupPlayTab();
    setupAnalyticsTab();

    tabWidget->addTab(automaticTab, "Automatic Solver");
    tabWidget->addTab(playTab, "Play Game");
    tabWidget->addTab(analyticsTab, "Analytics");

    loadDictionary();
}

MainWindow::~MainWindow() {
    delete solver;
    delete currentGame;
}

void MainWindow::setupAutomaticTab() {
    automaticTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(automaticTab);

    QFormLayout* formLayout = new QFormLayout();
    wordLengthComboAuto = new QComboBox();
    wordLengthComboAuto->addItems({ "3", "4" });
    formLayout->addRow("Word Length:", wordLengthComboAuto);

    startWordComboAuto = new QComboBox();
    formLayout->addRow("Start Word:", startWordComboAuto);

    targetWordComboAuto = new QComboBox();
    formLayout->addRow("Target Word:", targetWordComboAuto);
    layout->addLayout(formLayout);

    solveButton = new QPushButton("Solve");
    layout->addWidget(solveButton);
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::onSolveClicked);

    solutionList = new QListWidget();
    layout->addWidget(solutionList);

    connect(wordLengthComboAuto, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onWordLengthChanged);
}

void MainWindow::setupPlayTab() {
    playTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(playTab);

    QFormLayout* formLayout = new QFormLayout();
    playerNameEdit = new QLineEdit();
    formLayout->addRow("Player Name:", playerNameEdit);

    wordLengthComboPlay = new QComboBox();
    wordLengthComboPlay->addItems({ "3", "4", "5", "6", "7" });
    formLayout->addRow("Word Length:", wordLengthComboPlay);
    layout->addLayout(formLayout);

    QPushButton* startGameButton = new QPushButton("Start Game");
    layout->addWidget(startGameButton);
    connect(startGameButton, &QPushButton::clicked, this, &MainWindow::onStartPlayingClicked);

    QHBoxLayout* gameLayout = new QHBoxLayout();
    QVBoxLayout* leftLayout = new QVBoxLayout();

    currentWordLabel = new QLabel();
    currentWordLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(currentWordLabel);

    targetWordLabel = new QLabel();
    targetWordLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(targetWordLabel);

    wordInput = new QLineEdit();
    leftLayout->addWidget(wordInput);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    makeMoveButton = new QPushButton("Make Move");
    buttonLayout->addWidget(makeMoveButton);
    connect(makeMoveButton, &QPushButton::clicked, this, &MainWindow::onMakeMoveClicked);

    hintButton = new QPushButton("Get Hint");
    buttonLayout->addWidget(hintButton);
    connect(hintButton, &QPushButton::clicked, this, &MainWindow::onHintClicked);

    giveUpButton = new QPushButton("Give Up");
    buttonLayout->addWidget(giveUpButton);
    connect(giveUpButton, &QPushButton::clicked, this, &MainWindow::onGiveUpClicked);

    leftLayout->addLayout(buttonLayout);

    hintLabel = new QLabel();
    hintLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(hintLabel);

    statsLabel = new QLabel();
    statsLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(statsLabel);

    gameLayout->addLayout(leftLayout);

    gameLog = new QTextEdit();
    gameLog->setReadOnly(true);
    gameLayout->addWidget(gameLog);

    layout->addLayout(gameLayout);

    setGameControlsEnabled(false);
}

void MainWindow::setupAnalyticsTab() {
    analyticsTab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(analyticsTab);

    QFormLayout* formLayout = new QFormLayout();
    analyticsNameEdit = new QLineEdit();
    formLayout->addRow("Player Name:", analyticsNameEdit);
    layout->addLayout(formLayout);

    loadAnalyticsButton = new QPushButton("Load Analytics");
    layout->addWidget(loadAnalyticsButton);
    connect(loadAnalyticsButton, &QPushButton::clicked,
        this, &MainWindow::onLoadAnalyticsClicked);

    analyticsOutput = new QTextEdit();
    analyticsOutput->setReadOnly(true);
    layout->addWidget(analyticsOutput);
}

void MainWindow::loadDictionary() {
    try {
        int wordLength = wordLengthComboAuto->currentText().toInt();
        dictionary = GraphBuilder::loadDictionary(currentDictionaryFile.toStdString(), wordLength);

        startWordComboAuto->clear();
        targetWordComboAuto->clear();
        for (const auto& word : dictionary) {
            startWordComboAuto->addItem(QString::fromStdString(word));
            targetWordComboAuto->addItem(QString::fromStdString(word));
        }

        GraphBuilder builder;
        wordGraph = builder.buildGraph(dictionary);
        delete solver;
        solver = new Solver(wordGraph);
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error",
            QString("Failed to load dictionary: ") + e.what());
    }
}

void MainWindow::onWordLengthChanged(int index) {
    Q_UNUSED(index);
    loadDictionary();
}

void MainWindow::onSolveClicked() {
    QString start = startWordComboAuto->currentText();
    QString target = targetWordComboAuto->currentText();

    if (start == target) {
        QMessageBox::information(this, "Info", "Start and target words are the same");
        return;
    }

    auto path = solver->findShortestPath(start.toStdString(), target.toStdString());
    solutionList->clear();

    if (path.empty()) {
        solutionList->addItem("No path exists between these words");
    }
    else {
        for (const auto& word : path) {
            solutionList->addItem(QString::fromStdString(word));
        }
    }
}

void MainWindow::onStartPlayingClicked() {
    if (playerNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter your name");
        return;
    }

    if (currentGame) {
        endGame();
    }

    int wordLength = wordLengthComboPlay->currentText().toInt();
    dictionary = GraphBuilder::loadDictionary(currentDictionaryFile.toStdString(), wordLength);

    if (dictionary.size() < 2) {
        QMessageBox::warning(this, "Warning", "Not enough words in dictionary");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(dictionary.size()) - 1);

    int startIdx, targetIdx;
    do {
        startIdx = dist(gen);
        targetIdx = dist(gen);
    } while (startIdx == targetIdx ||
        solver->findShortestPath(dictionary[startIdx], dictionary[targetIdx]).empty());

    QString startWord = QString::fromStdString(dictionary[startIdx]);
    QString targetWord = QString::fromStdString(dictionary[targetIdx]);

    auto optimalPath = solver->findShortestPath(dictionary[startIdx], dictionary[targetIdx]);
    int optimalMoves = optimalPath.empty() ? 0 : optimalPath.size() - 1;

    currentGame = new GameSession(playerNameEdit->text(), startWord, targetWord, optimalMoves);
    currentGame->addMove(startWord);

    updateGameDisplay();
    gameLog->clear();
    gameLog->append(QString("Game started: %1 → %2").arg(startWord).arg(targetWord));
    gameLog->append("Make your first move!");

    setGameControlsEnabled(true);
    wordInput->setFocus();
}

void MainWindow::onMakeMoveClicked() {
    if (!currentGame) return;

    QString newWord = wordInput->text().toUpper();
    QString currentWord = currentGame->getCurrentWord();

    if (newWord.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a word");
        return;
    }

    if (newWord == currentWord) {
        QMessageBox::warning(this, "Warning", "New word is the same as current word");
        return;
    }

    int diffCount = 0;
    if (newWord.length() != currentWord.length()) {
        QMessageBox::warning(this, "Warning", "Word length must be the same");
        return;
    }

    for (int i = 0; i < newWord.length(); ++i) {
        if (newWord[i] != currentWord[i]) {
            diffCount++;
        }
    }

    if (diffCount != 1) {
        QMessageBox::warning(this, "Warning", "You must change exactly one letter");
        return;
    }

    if (std::find(dictionary.begin(), dictionary.end(), newWord.toStdString()) == dictionary.end()) {
        QMessageBox::warning(this, "Warning", "Word not in dictionary");
        return;
    }

    currentGame->addMove(newWord);
    wordInput->clear();
    hintLabel->clear();

    if (newWord == currentGame->getTargetWord()) {
        gameLog->append("Congratulations! You reached the target word.");
        endGame();
    }
    else {
        updateGameDisplay();
        gameLog->append(QString("Moved to: %1").arg(newWord));
    }
}

void MainWindow::onHintClicked() {
    if (!currentGame || currentGame->isComplete()) return;

    QString currentWord = currentGame->getCurrentWord();
    QString targetWord = currentGame->getTargetWord();

    auto hint = solver->getHint(currentWord.toStdString(), targetWord.toStdString());
    QString nextWord = QString::fromStdString(hint.first);
    int diffPos = hint.second;

    if (nextWord.isEmpty()) {
        hintLabel->setText("No hint available");
        return;
    }

    QString formattedCurrent = currentWord;
    if (diffPos >= 0 && diffPos < currentWord.length()) {
        formattedCurrent.replace(diffPos, 1,
            QString("<span style='color:red; font-weight:bold;'>%1</span>")
            .arg(currentWord[diffPos]));
    }

    QString formattedNext = nextWord;
    if (diffPos >= 0 && diffPos < nextWord.length()) {
        formattedNext.replace(diffPos, 1,
            QString("<span style='color:green; font-weight:bold;'>%1</span>")
            .arg(nextWord[diffPos]));
    }

    hintLabel->setTextFormat(Qt::RichText);
    hintLabel->setText(QString("Change %1 to %2")
        .arg(formattedCurrent)
        .arg(formattedNext));

    currentGame->incrementHints();
    updateGameDisplay();
    gameLog->append(QString("Hint used: change %1 to %2")
        .arg(currentWord)
        .arg(nextWord));
}

void MainWindow::onGiveUpClicked() {
    if (!currentGame) return;

    auto optimalPath = solver->findShortestPath(
        currentGame->getStartWord().toStdString(),
        currentGame->getTargetWord().toStdString());

    if (!optimalPath.empty()) {
        QStringList optimalPathQt;
        for (const auto& word : optimalPath) {
            optimalPathQt << QString::fromStdString(word);
        }

        gameLog->append("\nOptimal solution:");
        gameLog->append(optimalPathQt.join(" → "));
    }

    endGame();
}

void MainWindow::onLoadAnalyticsClicked() {
    QString username = analyticsNameEdit->text();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter a player name");
        return;
    }

    QList<GameSession> sessions = GameSession::loadFromFile(username);
    if (sessions.isEmpty()) {
        analyticsOutput->setText("No game data found for this player");
        return;
    }

    QString report;
    report += QString("Game sessions for %1:\n\n").arg(username);

    QSet<QString> uniqueWords;
    int totalGames = 0;
    int totalHints = 0;
    int totalMoves = 0;
    int totalOptimalMoves = 0;

    for (const auto& session : sessions) {
        totalGames++;
        totalHints += session.getHintsUsed();
        totalMoves += session.getMoveCount();

        auto optimalPath = solver->findShortestPath(
            session.getStartWord().toStdString(),
            session.getTargetWord().toStdString());
        int optimalMoves = optimalPath.empty() ? 0 : optimalPath.size() - 1;
        totalOptimalMoves += optimalMoves;

        for (const auto& word : session.getMoves()) {
            uniqueWords.insert(word);
        }

        report += QString("Game on %1:\n").arg(session.getStartTime().toString());
        report += QString("  %1 → %2\n").arg(session.getStartWord()).arg(session.getTargetWord());
        report += QString("  Moves: %1 (Optimal: %2)\n").arg(session.getMoveCount()).arg(optimalMoves);
        report += QString("  Hints used: %1\n\n").arg(session.getHintsUsed());
    }

    report += "\nSummary Statistics:\n";
    report += QString("Total games played: %1\n").arg(totalGames);
    report += QString("Unique words used: %1\n").arg(uniqueWords.size());
    report += QString("Average hints per game: %1\n").arg(totalHints / (double)totalGames, 0, 'f', 2);
    report += QString("Average moves per game: %1 (Optimal: %2)\n")
        .arg(totalMoves / (double)totalGames, 0, 'f', 2)
        .arg(totalOptimalMoves / (double)totalGames, 0, 'f', 2);
    report += QString("Efficiency: %1%\n")
        .arg(100 * totalOptimalMoves / (double)totalMoves, 0, 'f', 1);

    analyticsOutput->setText(report);
}

void MainWindow::updateGameDisplay() {
    if (!currentGame) return;

    currentWordLabel->setText(QString("Current word: <b>%1</b>")
        .arg(currentGame->getCurrentWord()));
    targetWordLabel->setText(QString("Target word: <b>%1</b>")
        .arg(currentGame->getTargetWord()));
    statsLabel->setText(QString("Moves: %1/%2 | Hints used: %3")
        .arg(currentGame->getMoveCount())
        .arg(currentGame->getOptimalMoves())
        .arg(currentGame->getHintsUsed()));
}

void MainWindow::endGame() {
    if (!currentGame) return;

    QString summary = QString("\nGame Summary:\n"
        "Player: %1\n"
        "Start: %2\n"
        "Target: %3\n"
        "Moves: %4 (Optimal: %5)\n"
        "Hints used: %6\n"
        "Your path: %7")
        .arg(currentGame->getPlayerName())
        .arg(currentGame->getStartWord())
        .arg(currentGame->getTargetWord())
        .arg(currentGame->getMoveCount())
        .arg(currentGame->getOptimalMoves())
        .arg(currentGame->getHintsUsed())
        .arg(currentGame->getMoves().join(" → "));

    gameLog->append(summary);
    currentGame->saveToFile();

    delete currentGame;
    currentGame = nullptr;
    setGameControlsEnabled(false);
}

void MainWindow::setGameControlsEnabled(bool enabled) {
    wordInput->setEnabled(enabled);
    makeMoveButton->setEnabled(enabled);
    hintButton->setEnabled(enabled);
    giveUpButton->setEnabled(enabled);

    QPalette palette = wordInput->palette();
    palette.setColor(QPalette::Base, enabled ? Qt::white : QColor(240, 240, 240));
    wordInput->setPalette(palette);

    if (enabled) {
        wordInput->setFocus();
        wordInput->setStyleSheet("color: black;");
    }
}