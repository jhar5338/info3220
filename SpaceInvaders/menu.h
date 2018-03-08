#ifndef MENU_H
#define MENU_H

#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

namespace game {
class Menu {
public:
    Menu(QWidget* parent, QString name, int& playeScore, QList<QPair<QString, int>> scores, int& width);
    ~Menu();
    void displayMenu(bool paused, bool dead);
    void settingsMenu(); // using camel case in this class since other vars use it from orig code
    void displayScoreboard(bool display);
    void addScore(QString name, int score);

private:
    QList<QPair<QString, int>> scores;
    QList<QPair<QLabel*,QLabel*>> scoreboard;
    int scoreboardY;
    void makeButtons(QWidget* parent, QString name);
    void closeButtons();  // if any buttons are left open, close them
    int& gameScore;
    QLabel* menu;
    int& width;
    QPushButton* restart;
    QPushButton* settings;
    QPushButton* slow;
    QPushButton* med;
    QPushButton* fast;
    QPushButton* black;
    QPushButton* white;
    QWidget* parent;

    QLabel* playerName;
    QLabel* playerScoreLabel;
    QLabel* speedLabel;
    QLabel* colourLabel;
    void revealPlayerScore(bool open);
    void sortScores();
    void createScoreboard(QWidget* parent);
    void writeScores();
};
}
#endif  // SCOREBUTTON_H
