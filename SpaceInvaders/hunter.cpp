#include "hunter.h"
namespace game {
Hunter::Hunter(QPixmap image, int x, int y, int velocity, int score, QString baseType, Base& ship,
        int stray)
        : Alien(image, x, y, velocity, score, baseType), stray(stray), ship(ship),
          baseType(baseType) {}

// determines which x axis to move
QString Hunter::calculateDirection() {
    if (ship.get_x() > this->get_x()) {
        return "R";
    } else if (ship.get_x() < this->get_x()) {
        return "L";
    }
    return "";
}

// will never listen to Swarm.
void Hunter::move(QString direction) {
    // calculate where the ship is
    QString move = calculateDirection();

    // always follow ship
    if (stray == 0) {
        Alien::move(move);
    } else {
        // decide if you want to be dumb
        int random = rand() % stray;
        if (random == 0) {
            Alien::move(move);
        } else {
            Alien::move(direction);
        }
    }
}

QList<Bullet*> Hunter::shoot(QString type) {
    return Alien::shoot(type);
}

// hunters expel 3 bullets when told they will be deleted
// if it is 'dumb' its bullets can kill other aliens
QList<Bullet*> Hunter::react() {
    QList<Bullet*> reaction;
    QString type = baseType;
    if (baseType == "dumb") {
        type = baseType + "Friendly";
    }
    reaction.append(Alien::shoot(type));
    reaction.append(Alien::shoot(type));
    reaction.append(Alien::shoot(type));
    return reaction;
}
}
