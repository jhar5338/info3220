#include "swarm.h"
#include "config.h"
#include <QPixmap>

// Also kind of like a builder for aliens.
namespace game {

/** CONSTRUCTOR
 * >>>>>> FOR AN EMPTY SWARM at init (e.g., root node)
 * it should ALWAYS shoot and move (see max pixels)
 * These following properties for Root swarm nodes are the defaults for
 * swarminfo
 *  - moves list is a single ""
 *  - maxPixels should be 0; because everything should always move
 * it should always shoot, if shootTime == 0; just shoot EVERYTHING
 * because lower swarms (that are not the root) decide for them selves anyway
 * leaf nodes from the root will always be shooting
 *  - Image will be some default e.g., redInvader
 *  - Type is as above, default is red
 *
 * >>>>>> Else, just fill out the swarm info and positions
 */

Swarm::Swarm(SwarmInfo& swarmInfo, Base& ship)
        : AlienBase(swarmInfo.swarmImage, 1, 0, 0, Config::getInstance()->get_SCALEDWIDTH(),
                  Config::getInstance()->get_SCALEDHEIGHT(), 0),
          maxPixels(10),  // in future, we can pass maxPixels in.
          currentPixel(0), currentMove(0), moves(swarmInfo.move), type(swarmInfo.type),
          shootTime(swarmInfo.shoot), shootCounter(0),
          builder(swarmInfo.swarmImage, swarmInfo.type, ship) {
    // make a new alien at each position
    // root node has an empty list :)
    for (QPair<int, int> position : swarmInfo.positions) {
        AlienBase* newAlien = builder.buildAlien(this->type, position.first, position.second);
        this->aliens.append(newAlien);
    }

    if (swarmInfo.positions.size() == 0) {
        // it's a root swarm. change MaxPixels
        maxPixels = 0;
    }
}

// must loop through its list recursively to delete everything
Swarm::~Swarm() {
    for (AlienBase* c : aliens) {
        delete c;
    }
}

bool Swarm::add(AlienBase* toAdd) {
    this->aliens.append(toAdd);
    return true;
}

void Swarm::remove(AlienBase* toDelete) {
    aliens.removeOne(toDelete);
    delete toDelete;
}

// direction should be "" to continue given instruct;
// if you want to force direction of swarm regardless of
// instruct then use "L", "R"...
void Swarm::move(QString direction) {
    if (direction != "") {
        overrideMove = direction;
    }

    if (this->currentPixel == this->maxPixels) {
        // move to next move, note we loop the movements.
        this->currentMove = (this->currentMove + 1) % this->moves.size();
        this->shootCounter++;
        this->currentPixel = 0;
        this->overrideMove = "";  // clear override
    }

    QString move = moves.at(currentMove);
    // if there is currently an override, replace the move
    if (overrideMove != "") {
        move = overrideMove;
    }

    // note: it might be a swarm not an alien.
    for (AlienBase* current : aliens) {
        current->move(move);
    }
    currentPixel++;
}

// shoot at the right time...
// Takes a 'type' because we can force it to shoot special bullets
// depending on the situation.
QList<Bullet*> Swarm::shoot(QString type) {
    QList<Bullet*> bullets;
    // if there are no aliens we can't shoot
    if (this->getAliens().size() == 0) {
        return bullets;
    }

    // ROOT composite swarm handler
    // alien can return a list too.
    if (this->shootTime == 0) {
        this->shootCounter = 0;  // so it doesn't overflow

        for (AlienBase*& c : this->aliens) {
            bullets.append(c->shoot(""));
        }
        return bullets;
    }

    // non root swarm composite.
    if (this->shootCounter >= this->shootTime) {
        // pick a random alien to shoot.
        int random = (rand() % aliens.size());
        AlienBase* chosen = aliens.at(random);
        // restart the counter randomly
        shootCounter = rand() % shootTime;
        QList<Bullet*> b = chosen->shoot(this->type + type);
        bullets.append(b);
    }

    return bullets;
}

// Getter...
QList<AlienBase*> Swarm::getAliens() const {
    return this->aliens;
}

int Swarm::get_score() const {
    return 0;
}

// how it reacts before it dies
QList<Bullet*> Swarm::react() {
    return QList<Bullet*>();  // does nothing.
}

void Swarm::paint(QPainter& painter) {
    for (auto* child : aliens) {
        child->paint(painter);
    }
}
}
