#pragma once
#include <QPixmap>

namespace game {
class Base {
private:
    QPixmap image; //this should be made to be a flyweight in future
    double scale;
    int x;
    int y;

protected:
    int boundaryX;
    int boundaryY;
    int minX;

public:
    // A BASE OBJECT CAN START ANYWHERE, WITH AN IMAGE AND SCALE.
    // THE DEFAULT Y VALUE IS SET TO 0
    Base(QPixmap image, double scale, int x, int y = 0, int boundaryX = 800, int boundaryY = 600,
            int minX = 0);

    // STAGE 2: Collision Check

    // collision checks
    bool checkXY(int x1, int x2, int myX);
    bool collides(int x1, int x2, int y1, int y2);
    bool collides(Base& base);

    // Setters
    void set_image(QPixmap image);
    void set_x(int x);
    void set_y(int y);

    // Getters
    const QPixmap& get_image() const;

    double get_scale() const;
    int get_x() const;
    int get_y() const;

    virtual ~Base();
};
}
