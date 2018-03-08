#pragma once
#include "base.h"
#include "bullet.h"
#include "bulletbuilderinterface.h"

namespace game {

class BulletBuilder : public BulletBuilderInterface {
    // THIS BUILDER DESIGN PATTERN ALLOWS FOR THE APPROPRIATE CREATION OF MULTIPLE
    // BULLETS
public:
    BulletBuilder(int velocity, Base& ship, QString baseType, bool friendly);

    void set_velocity(int velocity);
    Bullet* build_bullet(QString type);

    virtual ~BulletBuilder() {}

protected:
    QPixmap calculate_image(QString& type);
    bool friendly;
    int velocity;
    QPixmap image;
    QString baseType;
    Base& ship;
};
}
