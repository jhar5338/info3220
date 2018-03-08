#include "base.h"

namespace game {
Base::Base(QPixmap image, double scale, int x, int y, int boundaryX, int boundaryY, int minX)
        : image(image), scale(scale), x(x), y(y), boundaryX(boundaryX), boundaryY(boundaryY),
          minX(minX) {}

// check the coordinate is inside e.g., X inside Base width
bool Base::checkXY(int x1, int x2, int myX) {
    return (x1 <= myX && myX <= x2);
}

// check if any of the target's points are inside bullet coordinates
bool Base::collides(int x1, int x2, int y1, int y2) {
    // ugly....
    int myX1 = this->get_x();
    int myX2 = this->get_image().width() + myX1;
    int myY1 = this->get_y();
    int myY2 = this->get_image().height() + myY1;

    bool selfInsideTarget = (checkXY(x1, x2, myX1) && checkXY(y1, y2, myY1)) ||
                            (checkXY(x1, x2, myX2) && checkXY(y1, y2, myY1)) ||
                            (checkXY(x1, x2, myX1) && checkXY(y1, y2, myY2)) ||
                            (checkXY(x1, x2, myX2) && checkXY(y1, y2, myY2));
    // some Base in future might be larger than the target.
    bool targetInsideBullet = (checkXY(myX1, myX2, x1) && checkXY(myY1, myY2, y1)) ||
                              (checkXY(myX1, myX2, x1) && checkXY(myY1, myY2, y2)) ||
                              (checkXY(myX1, myX2, x2) && checkXY(myY1, myY2, y1)) ||
                              (checkXY(myX1, myX2, x2) && checkXY(myY1, myY2, y2));

    return (selfInsideTarget || targetInsideBullet);
}

bool Base::collides(Base& base) {
    return this->collides(base.get_x(), base.get_x() + base.get_image().width(), base.get_y(),
            base.get_y() + base.get_image().height());
}

// SETTERS
void Base::set_image(QPixmap image) {
    this->image = image;
}

// STAGE 2 Fix: checks first if it is out of window boundary
// before setting x or y
void Base::set_x(int x) {
    if (x > boundaryX) {
        this->x = boundaryX;
    } else if (x < minX) {
        this->x = minX;
    } else {
        this->x = x;
    }
}

void Base::set_y(int y) {
    if (y > boundaryY) {
        this->y = boundaryY;
    } else if (y < 0 - this->image.height()) {
        this->y = 0 - this->image.height();
    } else {
        this->y = y;
    }
}

// GETTERS
int Base::get_x() const {
    return this->x;
}

int Base::get_y() const {
    return this->y;
}

double Base::get_scale() const {
    return this->scale;
}

const QPixmap& Base::get_image() const {
    return this->image;
}

Base::~Base() {}
}
