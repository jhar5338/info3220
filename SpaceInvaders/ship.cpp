#include "ship.h"
#include "bullet.h"
#include "config.h"
#include <iostream>

namespace game {
Ship::Ship(QPixmap image, double scale, int x, int y)
        : Base(image, scale, x, y - image.height() * scale,
                  Config::getInstance()->get_SCALEDWIDTH(),
                  Config::getInstance()->get_SCALEDWIDTH(), 0),
          velocity(10), bullet_velocity(15), builder(bullet_velocity, *this, "laser", true) {
    // adjust the image size according to scale
    this->set_image(this->get_image().scaledToWidth(this->get_image().width() * scale));

    // readjust (x,y) since the image was resized
    boundaryX = Config::getInstance()->get_SCALEDWIDTH() - this->get_image().width();
    boundaryY = Config::getInstance()->get_SCALEDHEIGHT() - this->get_image().height();

    set_x(x);
    set_y(y);
}

// A SHIP CAN MOVE LEFT, RIGHT AND SHOOT
std::vector<Bullet*> Ship::shoot(bool super) {
    superState* s = new superState();
    defaultState* d = new defaultState();
    if (super) {
        setCurrentState(s);
        setPassiveState(d);
    } else {
        setCurrentState(d);
        setPassiveState(s);
    }
    std::vector<Bullet*>temp;
    if (m_currentState->processShoot()==1) {
        temp.push_back(builder.build_bullet("laser"));
    } else if (m_currentState->processShoot()==3) {
        for (int i = 0; i < 3; i++) {
            temp.push_back(builder.build_bullet("laser"));
        }
    }
    delete s;
    delete d;
    return temp;
}

// Setters
void Ship::move_left() {
    set_x(get_x() - velocity);
}

void Ship::move_right() {
    set_x(get_x() + velocity);
}

void Ship::setCurrentState(ShipState *currentState) {
    m_currentState = currentState;
}

void Ship::setPassiveState(ShipState *passiveState) {
    m_passiveState = passiveState;
}

void Ship::switchState() {
    //std::swap<ShipState*> (m_currentState,m_passiveState);
}

Ship::~Ship() {}
}
