#pragma once
#include "base.h"
#include "bullet.h"
#include "bulletbuilder.h"
#include <QPixmap>
#include <vector>
#include "defaultState.h"
#include "superState.h"

namespace game {

class Ship : public Base {
    // A SHIP CAN MOVE LEFT, RIGHT AND SHOOT (ALL WITH AN VELOCITY)

private:
    int velocity;
    int bullet_velocity;
    BulletBuilder builder;

    ShipState *m_currentState;
    ShipState *m_passiveState;

public:
    Ship(QPixmap image, double scale, int x, int y);
    std::vector<Bullet*> shoot(bool super);
    void move_left();
    void move_right();

    void setCurrentState(ShipState *currentState);
    void setPassiveState(ShipState *passiveState);
    void switchState();

    virtual ~Ship();
};
}
