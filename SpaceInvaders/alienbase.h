#pragma once

#include "base.h"
#include "bullet.h"
#include <QPainter>

// An abstract for the Swarm; note the move() function.
namespace game {
class AlienBase : public Base {
public:
    AlienBase(
            QPixmap image, double scale, int x, int y, int windowWidth, int windowHeight, int minX)
            : Base(image, scale, x, y, windowWidth, windowHeight, minX) {}
    virtual ~AlienBase() {}

    virtual QList<Bullet*> shoot(QString type) = 0;
    virtual void move(QString direction) = 0;
    virtual int get_score() const = 0;
    // if it is a leaf, it will have an empty list for the func below.
    virtual QList<AlienBase*> getAliens() const = 0;
    virtual bool add(AlienBase* toAdd) = 0;
    virtual void remove(AlienBase* toDelete) = 0;
    virtual QList<Bullet*> react() = 0;
    virtual void paint(QPainter& painter) = 0;
};
}
