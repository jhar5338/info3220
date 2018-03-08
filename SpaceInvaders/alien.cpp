#include "alien.h"
#include "config.h"

namespace game {
Alien::Alien(QPixmap image, int x, int y, int velocity, int score, QString baseType)
        : AlienBase(image, 1, x, y, Config::getInstance()->get_SCALEDWIDTH() - image.width(),
                  Config::getInstance()->get_SCALEDHEIGHT(), 0),
          velocity(velocity), score(score), bulletSFX(),
          builder(-15, *this, baseType + "InvaderLaser", false) {

    //BULLET SOUND EFFECTS
    bulletSFX.setSource(QUrl::fromLocalFile(":/Sounds/invader.wav"));
    bulletSFX.setVolume(0.3f);
}

// aliens only move L or R right now, perhaps in future U D (up,down)
void Alien::move(QString move) {
    if (move == "L") {
        set_x(get_x() - velocity);
    } else if (move == "R") {
        set_x(get_x() + velocity);
    } else if (move == "U") {
        set_y(get_y() - velocity);
    } else if (move == "D") {
        set_y(get_y() + velocity);
    }
}

// returns a list of 1 bullet (perhaps you could change this later)
QList<Bullet*> Alien::shoot(QString type) {
    // randomly generates a diagonal bullet - in future, you could have
    // a more determined way to generate them instead of just randomly doing it...
    int random = rand() % 3;
    bulletSFX.play();
    QList<Bullet*> bullets;
    if (random == 0) {
        bullets.append(builder.build_bullet(type + "InvaderLaserRotateLeft"));
    } else if (random == 1) {
        bullets.append(builder.build_bullet(type + "InvaderLaserRotateRight"));
    } else {
        bullets.append(builder.build_bullet(type + "InvaderLaser"));
    }

    return bullets;
}

QList<Bullet*> Alien::react() {
    return QList<Bullet*>();
}

// getters
int Alien::get_score() const {
    return this->score;
}

// Aliens are a leaf type component, so no children.
// Since there are no children we don't have to worry
// about returning a local variable.
QList<AlienBase*> Alien::getAliens() const {
    return QList<AlienBase*>();
}

bool Alien::add(AlienBase*) {
    return false;
}

void Alien::remove(AlienBase*) {
    return;
}

void Alien::paint(QPainter& painter) {
    painter.drawPixmap(get_x(), get_y(), get_image());
}
}
