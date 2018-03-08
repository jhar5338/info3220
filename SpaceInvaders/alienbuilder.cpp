#include "alienbuilder.h"
namespace game {

AlienBuilder::AlienBuilder(QPixmap& image, QString& type, Base& ship)
        : ship(ship), image(image), type(type) {
    this->velocity = velocityCalculator(type);
    this->score = scoreCalculator(type);
}

// asumes the passed in type is correct already.
// Since we call this in game.
Alien* AlienBuilder::buildAlien(QString& type, int x, int y) {
    QPixmap currentImage = this->image;
    int currentVelocity = this->velocity;
    int currentScore = this->score;

    Alien* newAlien;
    if (type != this->type) {
        // override image, velocity, score
        currentImage.load(":/Images/" + type + "Invader.png");
        currentVelocity = velocityCalculator(type);
        currentScore = scoreCalculator(type);
    }

    int stray = -1;
    if (type == "hunter") {
        stray = 0;
    } else if (type == "dumb") {
        stray = 3;
    }

    if (stray != -1) {
        newAlien = new Hunter(currentImage, x, y, currentVelocity, currentScore, type, ship, stray);
        return newAlien;
    }

    newAlien = new Alien(currentImage, x, y, currentVelocity, currentScore, type);
    return newAlien;
}

// calculate alien velocity based on the type.
int AlienBuilder::velocityCalculator(QString& type) {
    if (type == "red") {
        return 1;
    } else if (type == "blue") {
        return 5;
    } else if (type == "hunter") {
        return 3;
    } else if (type == "dumb") {
        return 6;
    }
    return 1;
}

// calculate score of alien based on their type.
int AlienBuilder::scoreCalculator(QString& type) {
    if (type == "red") {
        return 1;
    } else if (type == "blue") {
        return 3;
    } else if (type == "hunter") {
        return 5;
    } else if (type == "dumb") {
        return 2;
    }
    return 1;
}
}
