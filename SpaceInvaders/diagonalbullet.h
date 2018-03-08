#ifndef DIAGONALBULLET_H
#define DIAGONALBULLET_H
#include "bullet.h"
#include <QPixmap>

namespace game {
class DiagonalBullet : public Bullet {
protected:
    bool right;  // move x axis left or right...
public:
    DiagonalBullet(QPixmap image, int x, int y, int bullet_velocity, bool friendly, bool right);
    virtual ~DiagonalBullet() {}

    void move();
};
}
#endif  // DIAGONALBULLET_H
