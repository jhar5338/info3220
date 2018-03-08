#include "diagonalbullet.h"

namespace game {

DiagonalBullet::DiagonalBullet(
        QPixmap image, int x, int y, int bullet_velocity, bool friendly, bool right)
        : Bullet(image, x, y, bullet_velocity, friendly), right(right) {
    // The base image will be rotated by the builder.
    // Because builders construct the complex parts :)
}

void DiagonalBullet::move() {
    if (right) {
        this->set_x(this->get_x() - get_bullet_velocity());
    } else {
        this->set_x(this->get_x() + get_bullet_velocity());
    }

    this->set_y(this->get_y() - get_bullet_velocity());
}
}
