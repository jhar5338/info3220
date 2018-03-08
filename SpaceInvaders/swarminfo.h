#ifndef SWARMINFO_H
#define SWARMINFO_H
#include <QList>
#include <QPair>
#include <QPixmap>
#include <QString>
#include <QStringList>
namespace game {
class SwarmInfo {
public:
    SwarmInfo(QString type, QList<QPair<int, int>> positions, QStringList move, int shoot);
    SwarmInfo();
    ~SwarmInfo() {}

    QPixmap swarmImage;
    QString type;
    QList<QPair<int, int>> positions;
    QStringList move;
    int shoot;
};
}
#endif  // SWARMINFO_H
