#pragma once
#include "base.h"

namespace game {

class Bullet : public Base {
    // A BULLET THAT INHERITS FROM THE BASE, HAS THE ADDITIONAL ATTRIBUTE OF A
    // BULLET VELOCITY.

private:
    int bullet_velocity;
    bool friendly;

public:
    Bullet(QPixmap image, int x, int y, int bullet_velocity, bool friendly);
    virtual void move();
    virtual ~Bullet();
    int get_bullet_velocity() const;
    bool isFriendly();
};
}
