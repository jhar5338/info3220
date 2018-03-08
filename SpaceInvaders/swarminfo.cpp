#include "swarminfo.h"
namespace game {
// basically a 'struct' for swarms. Except I called it 'class' anyway. Theyre
// basically the same, anyway...
SwarmInfo::SwarmInfo(QString type, QList<QPair<int, int>> positions, QStringList move, int shoot)
        : type(type), positions(positions), move(move), shoot(shoot) {
    swarmImage.load(":/Images/" + type + "Invader.png");
}

// DEFAULT swarmInfo constructor
SwarmInfo::SwarmInfo() : type("red"), shoot(0) {
    // leave positions empty.
    swarmImage.load(":/Images/" + type + "Invader.png");
    ;
    move << "";
}
}
