#include "menu.h"
#include <iostream>

namespace game {
Menu::Menu(QWidget* parent, QString name, int& playerScore, QList<QPair<QString, int>> scores, int& width)
        : scores(scores),gameScore(playerScore), width(width), parent(parent) {
    createScoreboard(this->parent);
    makeButtons(parent, name);
    createScoreboard(parent);
}

Menu::~Menu() {
    delete menu;
    delete playerName;
    delete playerScoreLabel;
    delete restart;
    delete speedLabel;
    delete slow;
    delete med;
    delete fast;
    delete colourLabel;
    delete black;
    delete white;
    for (int i = 0; i < scoreboard.size(); i++) {
        delete scoreboard.at(i).first;
        delete scoreboard.at(i).second;
    }
}

void Menu::makeButtons(QWidget* parent, QString name) {

    // pause menu
    menu = new QLabel("Paused",parent);
    menu->setGeometry(QRect((width/2)-50, 50, 100, 30));
    menu->setVisible(false);
    menu->setStyleSheet("background-color: yellow");
    menu->setAlignment(Qt::AlignCenter);

    restart = new QPushButton("Restart", parent);
    restart->setGeometry(QRect((width/2)-50, 90, 100, 30));
    restart->setVisible(false);
    restart->setStyleSheet("background-color: green");
    QObject::connect(restart, SIGNAL(released()), parent, SLOT(restart()));

    settings = new QPushButton("Settings", parent);
    settings->setGeometry(QRect((width/2)-50, 130, 100, 30));
    settings->setVisible(false);
    settings->setStyleSheet("background-color: green");
    QObject::connect(settings, SIGNAL(released()), parent, SLOT(settings()));

    // current score
    playerName = new QLabel(parent);
    playerName->setGeometry((width/2)-100, 170, 100, 30);
    playerName->setText(name);
    playerName->setVisible(false);
    playerName->setStyleSheet("background-color: white");
    playerName->setAlignment(Qt::AlignCenter);

    playerScoreLabel = new QLabel(parent);
    playerScoreLabel->setGeometry(width/2, 170, 100, 30);
    playerScoreLabel->setText(QString::number(gameScore));
    playerScoreLabel->setVisible(false);
    playerScoreLabel->setStyleSheet("background-color: white");
    playerScoreLabel->setAlignment(Qt::AlignCenter);

    // settings menu
    speedLabel = new QLabel(parent);
    speedLabel->setGeometry((width/2)-50, 50, 100, 30);
    speedLabel->setText("Speed");
    speedLabel->setVisible(false);
    speedLabel->setStyleSheet("background-color: yellow");
    speedLabel->setAlignment(Qt::AlignCenter);

    slow = new QPushButton("Slow", parent);
    slow->setGeometry(QRect((width/2)-180, 90, 100, 30));
    slow->setVisible(false);
    slow->setStyleSheet("background-color: green");
    QObject::connect(slow, SIGNAL(released()), parent, SLOT(slow()));

    med = new QPushButton("Medium", parent);
    med->setGeometry(QRect((width/2)-50, 90, 100, 30));
    med->setVisible(false);
    med->setStyleSheet("background-color: green");
    QObject::connect(med, SIGNAL(released()), parent, SLOT(med()));

    fast = new QPushButton("Fast", parent);
    fast->setGeometry(QRect((width/2)+80, 90, 100, 30));
    fast->setVisible(false);
    fast->setStyleSheet("background-color: green");
    QObject::connect(fast, SIGNAL(released()), parent, SLOT(fast()));

    colourLabel = new QLabel(parent);
    colourLabel->setGeometry((width/2)-50, 130, 100, 30);
    colourLabel->setText("Background");
    colourLabel->setVisible(false);
    colourLabel->setStyleSheet("background-color: yellow");
    colourLabel->setAlignment(Qt::AlignCenter);

    black = new QPushButton("Black", parent);
    black->setGeometry(QRect((width/2)-110, 170, 100, 30));
    black->setVisible(false);
    black->setStyleSheet("background-color: green");
    QObject::connect(black, SIGNAL(released()), parent, SLOT(black()));

    white = new QPushButton("White", parent);
    white->setGeometry(QRect((width/2)+10, 170, 100, 30));
    white->setVisible(false);
    white->setStyleSheet("background-color: green");
    QObject::connect(white, SIGNAL(released()), parent, SLOT(white()));

}

// called when game is paused or unpaused
void Menu::displayMenu(bool paused, bool dead) {
    if (!paused) {
        closeButtons();
    } else {
        if (dead) {
            menu->setText("Dead");
            menu->setStyleSheet("background-color: red");
        } else {
            menu->setText("Paused");
            menu->setStyleSheet("background-color: yellow");
        }
        menu->setVisible(true);
        revealPlayerScore(true);
        restart->setVisible(true);
        settings->setVisible(true);
        displayScoreboard(true);
    }
}

// helper, closes all the opened menus
void Menu::closeButtons() {
    menu->setVisible(false);
    revealPlayerScore(false);
    restart->setVisible(false);
    settings->setVisible(false);
    speedLabel->setVisible(false);
    slow->setVisible(false);
    med->setVisible(false);
    fast->setVisible(false);
    colourLabel->setVisible(false);
    black->setVisible(false);
    white->setVisible(false);
    displayScoreboard(false);
}

void Menu::revealPlayerScore(bool open) {
    // recalculate playerScoreLabel
    playerScoreLabel->setText(QString::number(gameScore));
    playerName->setVisible(open);
    playerScoreLabel->setVisible(open);
}

// opens settings menu
void Menu::settingsMenu() {
    closeButtons();
    speedLabel->setVisible(true);
    slow->setVisible(true);
    med->setVisible(true);
    fast->setVisible(true);
    colourLabel->setVisible(true);
    black->setVisible(true);
    white->setVisible(true);
}

// sorts scores list using bubble sort since max 5 elements
void Menu::sortScores() {
    int i = 0;
    bool swap = true; // true to begin first pass
    while (swap) {
        swap = false;
        for (i = 0; i < scores.size()-1; i++) {
            if (scores.at(i).second < scores.at(i+1).second) {
                scores.swap(i,i+1);
                swap = true;
            }
        }
    }
}

// creates label objects for scoreboard and stores pointers in pairs in scoreboard list
void Menu::createScoreboard(QWidget* parent) {
    scores.clear();
    for (int i = 0; i < scoreboard.size(); i++) {
        delete scoreboard.at(i).first;
        delete scoreboard.at(i).second;
    }
    scoreboard.clear();
    // Read scores file
    QFile s_file("../SpaceInvaders/scores.txt");
    s_file.open(QIODevice::ReadOnly);

    QTextStream in(&s_file);
    QString line;
    scoreboardY = 210; // current score Y val is 170, scoreboard firstY val should be 40 more hence 210
    // Read each line until the maximum scores are taken (five scores)
    while (!in.atEnd()) {
        line = in.readLine();
        line = line.trimmed();
        if (line.isEmpty()) {
            continue;
        } else {
            QPair<QString,int> curr;
            curr.first = line.split(",").first();
            curr.second = line.split(",").last().toInt();
            this->scores.append(curr);
            if (this->scores.size()==5) {
                break;
            }

        }
    }
    s_file.close();
    // sort if more than one score
    if (this->scores.size() > 1) {
        sortScores();
    }
    writeScores();
    // for each score create a label for name and score then add to scoreboard list
    for (QPair<QString, int> curr : scores) {
        QLabel* player = new QLabel(parent);
        QLabel* score = new QLabel(parent);
        player->setGeometry(QRect((width/2)-100, scoreboardY, 100, 30));
        player->setText(curr.first);
        player->setVisible(false);
        player->setStyleSheet("background-color: white");
        player->setAlignment(Qt::AlignCenter);

        score->setGeometry(QRect(width/2, scoreboardY, 100, 30));
        score->setText(QString::number(curr.second));
        score->setVisible(false);
        score->setStyleSheet("background-color: white");
        score->setAlignment(Qt::AlignCenter);
        scoreboardY+=30;

        QPair<QLabel*,QLabel*> temp;
        temp.first = player;
        temp.second = score;
        scoreboard.append(temp);
    }
    scoreboardY = 210;
}

// toggles scoreboard
void Menu::displayScoreboard(bool display) {
    if (display) {
        for (QPair<QLabel*,QLabel*> curr : scoreboard) {
            curr.first->setVisible(true);
            curr.second->setVisible(true);
        }
    } else {
        for (QPair<QLabel*,QLabel*> curr : scoreboard) {
            curr.first->setVisible(false);
            curr.second->setVisible(false);
        }
    }
}

// write scores to file
void Menu::writeScores() {
    QFile s_file("../SpaceInvaders/scores.txt");
    s_file.resize(0);
    s_file.open(QIODevice::WriteOnly);

    QTextStream out(&s_file);
    QString line;

    for (QPair<QString,int> curr : scores) {
        line = curr.first;
        line.append("," + QString::number(curr.second));
        out<<line<<endl;
    }
    s_file.close();
}

/*
 * add new score, score file is sorted so if new score is higher than lowest score in top 5,
 * 5th place score will be replaced
 */
void Menu::addScore(QString name, int score) {
    QFile s_file("../SpaceInvaders/scores.txt");
    s_file.open(QIODevice::ReadWrite);

    QTextStream stream(&s_file);
    QString line;
    int counter = 0;
    QString newScore = name;
    newScore.append("," + QString::number(score));
    while (!stream.atEnd()) {
        line = stream.readLine();
        counter++;
        if (counter==5) {
            // if smallest score is smaller than new score then replace old with new
            if (line.split(",").last().toInt()<score) {
                s_file.resize(0);
                s_file.close();
                s_file.open(QIODevice::WriteOnly);
                QTextStream out (&s_file);
                QString line;

                // only add highest 4 scores
                for (int i = 0; i < 4; i++) {
                    line = scores.at(i).first;
                    line.append("," + QString::number(scores.at(i).second));
                    stream<<line<<endl;
                }
                stream<<newScore<<endl; // enter last score
            }
            break;
        }
    }
    if (counter<5) {
        stream<<newScore<<endl; // add score if at end of file before reached a 5th score
        s_file.close();
    }
    createScoreboard(this->parent);
    displayScoreboard(true);
}
}
