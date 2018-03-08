#include "bulletbuilder.h"
#include "base.h"
#include "bullet.h"
#include "diagonalbullet.h"

namespace game {
// extended it s.t., there is a base bullet e.g., Ship has base of 'laser'
// and aliens might have an 'alien laser' base type
BulletBuilder::BulletBuilder(int velocity, Base& ship, QString baseType, bool friendly)
        : friendly(friendly), velocity(velocity), baseType(baseType), ship(ship) {
    this->image = calculate_image(baseType);
}

Bullet* BulletBuilder::build_bullet(QString type) {
    QPixmap currentImage;
    bool currentFriendly = this->friendly;

    if (baseType == type) {
        currentImage = this->image;
    } else {
        currentImage = calculate_image(type);
    }

    if (type.contains("Friendly")) {
        currentFriendly = true;
    }

    // rotates if necessary
    if (type.contains("RotateRight")) {
        return new DiagonalBullet(currentImage,
                ship.get_x() + ship.get_image().width() * 0.5 - (currentImage.width() * 0.5),
                ship.get_y() + currentImage.height() * 0.5, this->velocity, currentFriendly, true);
    } else if (type.contains("RotateLeft")) {
        return new DiagonalBullet(currentImage,
                ship.get_x() + ship.get_image().width() * 0.5 - (currentImage.width() * 0.5),
                ship.get_y() + currentImage.height() * 0.5, this->velocity, currentFriendly, false);
    }

    // EXTENSION 4 - ALIGN BULLET ACCORDING TO SHIP SIZE REGARDLESS OF STARTING
    // POSITION.
    // STAGE 2 : i fixed this; before it was 12 * scale for some reason.
    return new Bullet(currentImage,
            ship.get_x() + ship.get_image().width() * 0.5 - (currentImage.width() * 0.5),
            ship.get_y() + currentImage.height() * 0.5, this->velocity, currentFriendly);
}

QPixmap BulletBuilder::calculate_image(QString& type) {
    QPixmap currentImage;
    if (type.contains("red")) {
        currentImage.load(":/Images/redInvaderLaser.png");
    } else if (type.contains("blue")) {
        currentImage.load(":/Images/blueInvaderLaser.png");
    } else if (type.contains("dumb")) {
        currentImage.load(":/Images/dumbInvaderLaser.png");
    } else if (type.contains("hunter")) {
        currentImage.load(":/Images/hunterInvaderLaser.png");
    } else {
        // default is a laser i.e., player ship
        currentImage.load(":/Images/laser.png");
    }

    // check for rotation!
    int angle = 0;
    if (type.contains("RotateRight")) {  // should angle to the shooter's right
        angle = -45;
    } else if (type.contains("RotateLeft")) {  // rotate to shooter's left
        angle = 45;
    }

    if (angle != 0) {
        // rotate, and generate 'diagonal bullet' instead
        QTransform transform;
        QTransform rotate = transform.rotate(angle);
        currentImage = QPixmap(currentImage.transformed(rotate));
    }

    return currentImage;
}

// SETTER
void BulletBuilder::set_velocity(int velocity) {
    this->velocity = velocity;
}
}
