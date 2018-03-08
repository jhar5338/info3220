#pragma once
#include "swarminfo.h"
#include <QFile>
#include <QHash>
#include <QList>
#include <QPair>
#include <QTextStream>
#define NAME_LENGTH 5

namespace game {

class Config {
private:
    // methods
    static Config* s_configInstance;

    void processShip(QTextStream& in);
    void processConfigSizeLine(QString l);
    void processInstructions(QStringList& instructs, QStringList& copy);

    void processSwarm(QTextStream& in);
    void processPairs(QStringList list, QList<QPair<int, int>>& positions);
    void processMoves(QStringList& move, QStringList& list);
    void saveSwarm(QString type, QList<QPair<int, int>> positions, QStringList move, int shoot);
    void scalePositions();
    // SHIP values
    QString name;  // this will be useful in Stage 3 ;)
    double scale;
    int startpos;
    int frames;
    QStringList instructs;
    QString defaultInstructs;
    int SCALEDWIDTH;
    int SCALEDHEIGHT;
    QString control; // ship controlled by user or config
    int level;

    // Swarm Info Map
    QList<SwarmInfo> swarmList = {};

    // default Alien values
    QStringList alienTypes;
    QString defaultAlienType;
    QStringList defaultAlienMove;
    const int defaultAlienShoot = 10;

    void initDefault();

protected:
    Config();

public:
    // AS THE CONFIG GIVES INPUT, THE FILE IS READ TO THEN SET ATTRIBUTES.
    // GETTERS
    static Config* getInstance();
    ~Config();
    QString get_name();

    double get_scale();
    int get_startpos();
    QStringList get_instructs();
    QList<SwarmInfo> getSwarmList();
    int get_frames();
    void next_level();

    int get_SCALEDWIDTH();
    int get_SCALEDHEIGHT();
    QString get_control();
    void set_level(int level);
};
}
