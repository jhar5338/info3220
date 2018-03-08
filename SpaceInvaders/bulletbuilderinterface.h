#pragma once
#include "bullet.h"
namespace game {

class BulletBuilderInterface {
public:
    virtual Bullet* build_bullet(QString type) = 0;
    virtual void set_velocity(int velocity) = 0;
};
}
