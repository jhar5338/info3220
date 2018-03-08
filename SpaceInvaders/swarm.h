#ifndef SWARM_H
#define SWARM_H
#include "alien.h"
#include "alienbase.h"
#include "alienbuilder.h"
#include "bullet.h"
#include "swarminfo.h"
#include <QList>

// Composite of Aliens
// Controls the entire fleet's movement and what they shoot
namespace game {
class Swarm : public AlienBase {
private:
    int maxPixels;     // how many pixels it should move for each instruction
    int currentPixel;  // which pixel we are up to.
    int currentMove;
    QStringList moves;
    QString overrideMove;

    QString type;
    int shootTime;
    int shootCounter;
    QList<AlienBase*> aliens;
    AlienBuilder builder;

public:
    Swarm(SwarmInfo& swarmInfo, Base& ship);
    ~Swarm();

    bool add(AlienBase* toAdd);
    void remove(AlienBase* toDelete);

    // inherited stuff
    void move(QString direction);
    QList<Bullet*> shoot(QString type);
    int get_score() const;
    QList<AlienBase*> getAliens() const;
    QList<Bullet*> react();
    virtual void paint(QPainter& painter);
};
}
#endif  // SWARM_H
