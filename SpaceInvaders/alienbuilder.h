#ifndef ALIENBUILDER_H
#define ALIENBUILDER_H
#include "alien.h"
#include "hunter.h"
#include <QPixmap>
#include <QString>

namespace game {
class AlienBuilder {
private:
    Base& ship;

protected:
    QPixmap image;
    QString type;
    int score;
    int velocity;
    int id;

public:
    // initiates default alien to be built
    AlienBuilder(QPixmap& image, QString& type, Base& ship);
    ~AlienBuilder() {}
    // input type can override default alien on the production line for this alien
    Alien* buildAlien(QString& type, int x, int y);
    int velocityCalculator(QString& type);
    int scoreCalculator(QString& type);
};
}

#endif  // ALIENBUILDER_H
