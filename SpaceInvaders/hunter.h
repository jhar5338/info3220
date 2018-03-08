#ifndef HUNTER_H
#define HUNTER_H
#include "alien.h"

namespace game {
class Hunter : public Alien {
public:
    Hunter(QPixmap image, int x, int y, int velocity, int score, QString baseType, Base& ship,
            int stray);

    ~Hunter() {}

    void move(QString direction);
    QList<Bullet*> shoot(QString type);
    QList<Bullet*> react();

protected:
    int stray;
    Base& ship;
    QString baseType;
    QString calculateDirection();
};
}
#endif  // HUNTER_H
