#include "alienbase.h"

namespace game {

AlienBase::AlienBase(
        QPixmap image, double scale, int x, int y, int windowWidth, int windowHeight, int minX)
        : Base(image, scale, x, y, windowWidth, windowHeight, minX) {}
}
