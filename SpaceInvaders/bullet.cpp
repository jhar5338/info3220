#include "bullet.h"
#include "config.h"

namespace game {
Bullet::Bullet(QPixmap image, int x, int y, int bullet_velocity, bool friendly)
        : Base(image, 1, x, y, Config::getInstance()->get_SCALEDWIDTH(),
                  Config::getInstance()->get_SCALEDHEIGHT(), 0 - image.width()),
          bullet_velocity(bullet_velocity), friendly(friendly) {}

Bullet::~Bullet() {}

// SETTER
void Bullet::move() {
    set_y(get_y() - get_bullet_velocity());
}

int Bullet::get_bullet_velocity() const {
    return this->bullet_velocity;
}

bool Bullet::isFriendly() {
    return this->friendly;
}
}
