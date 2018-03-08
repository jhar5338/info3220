
#include "config.h"
#include <QChar>
#include <iostream>
#define WIDTH 800
#define HEIGHT 600

/** Commentary added at Stage 2
 * I copied the original kind of format for reading... lots of if/else
 * statements.
 * Made it a bit easier to extend
 * SHIP - read with lots of IF ELSE statements.
 *
 * ==========================================================================================
 * List of aliens: actually a list of swarms, and swarms are composites of
 * aliens.
 * Aliens are read like this:
 * position= 0 90, 0 100, 32 99 -> "format X[space]Y"; in other words X Y, X Y,
 * X Y
 *              -> positions are adjusted during reading.
 *              i.e., if X,Y are NOT in window boundary, they will be set to the
 * boundary.
 *              & if X or Y is excluded (can't tell which one) ignore that
 * entry.
 *              -> if there is an empty space (e.g. '__,__,') where _ is blank
 * space
 *                  it will skip it.
 *              ->We can have overlapping aliens for an interesting effect.
 *              ->Of course, numAliens is numPositions.
 * type=red -> e.g., red, blue, hunter... velocity is calculated in Builder.
 * move=L,R,L,L (shoot is determined randomly within the swarm). If empty fills
 * with a default move..
 * shoot=5 -> (at each move, swarms decrement this number until 0 and then 1
 * alien shoots).
 *              -> when it is 0, shoot will be then reset between 0~5 again.
 * [see Swarm]
 *
 * ==========================================================================================
 *  " How do we know when properties belong to a swarm, vs a different swarm or
 * player ship?"
 * -> the header [SHIP] will be used for the player
 *               [SWARM] for swarms
 *               [STAGE] when stage 3 of assignment is implemented.
 * -> E.g., let's say we read [SHIP]. Enter 'SHIP' saving state.
 *  Lets say, we read position=....
 * but position is an ALIEN property, not player ship! Will just skip it.
 * Keep reading until you reach another header [SWARM] or [STAGE].
 * -> if there is another [SHIP] ignore it and print a complaint.
 * -> for [SWARM] there are multiple. So just start reading the next swarm.
 *
 * */

namespace game {

Config* Config::s_configInstance = 0;

Config::Config() {
    // GENERAL READING OF CONFIG FILE
    QFile c_file("../SpaceInvaders/config1.txt");
    c_file.open(QIODevice::ReadOnly);

    QTextStream in(&c_file);
    initDefault();
    // READ IN CONFIG FOR CUSTOM SETTINGS
    while (!in.atEnd()) {
        // assumes ship first, but it's okay anyway [just read it]
        processShip(in);
    }
    // set the scaled width and height!
    this->SCALEDWIDTH = WIDTH * this->scale;
    this->SCALEDHEIGHT = HEIGHT * this->scale;
    // scales the alien positions based on the scale.
    scalePositions();
    level = 1;
}

// loops through all the swarm info and edits positions.
void Config::scalePositions() {
    for (SwarmInfo& s : swarmList) {
        for (QPair<int, int> pos : s.positions) {
            pos = qMakePair(pos.first * this->scale, pos.second * this->scale);
        }
    }
}

Config::~Config() {
    delete this->s_configInstance;
}

// DEFAULT CONFIG SETTING
void Config::initDefault() {
    scale = 1;
    startpos = 150;
    frames = 40;  // changed this to 100 because 1000 is super slow
    defaultInstructs = "Shoot";
    control = "config";
    instructs << defaultInstructs;
    name = "PLAY1";

    alienTypes << "red"
               << "blue"
               << "hunter"
               << "dumb";
    defaultAlienType = "red";
    defaultAlienMove << "L"
                     << "L"
                     << "R";
}

// MAKE SINGLETON INSTANCE
Config* Config::getInstance() {
    if (s_configInstance != 0) {
        return s_configInstance;
    } else {
        s_configInstance = new Config();
        return s_configInstance;
    }
}

// Process [SHIP] information
void Config::processShip(QTextStream& in) {
    while (!in.atEnd()) {
        QString l = in.readLine();
        l = l.trimmed();

        if (l.isEmpty()) {
            continue;
        } else if (l.startsWith("size=")) {
            l = l.split("=").last();
            processConfigSizeLine(l);
        } else if (l.startsWith("x=")) {  // starts with START X POSITION

            l = l.split("=").last();

            if (l.toInt() != 0) {
                startpos = l.toInt();
            }
        } else if (l.startsWith("name=")) {
            l = l.split("=").last();
            if (l.length() != 0) {
                name = l.split("=").last();  // name can be anything, up to 5 char long.
                name.chop(name.size() - NAME_LENGTH);
                name = name.toUpper();
            }

        } else if (l.startsWith("instructions=")) {  // starts with movement INSTRUCTIONS
            l = l.split("=").last();
            QStringList copy = l.split(",");
            processInstructions(instructs, copy);

        } else if (l.startsWith("frames=")) {
            // CUSTOMISE USER EXPERIENCE BY SETTING CUSTOM FRAMES PER SECOND
            l = l.split("=").last();
            // 0 is 'non digit'.
            if (l.toInt() != 0) {
                frames = l.toInt();
            }
        } else if (l.startsWith("[SWARM]")) {
            // reads a different header.
            processSwarm(in);
            return;  // we already have default settings for ship so it's okay to just
                     // return
        } else if (l.startsWith("[SHIP]")) {
            // ignore [SHIP] line
        } else if (l.startsWith("control=")) {
            l = l.split("=").last();
            if (l.length() != 0) {
                control = l;
            }
        } else {
            std::cout << "Incorrect key, check [SHIP] usage" << std::endl;
            std::cout << "<" << l.toStdString() << ">" << std::endl;
        }
    }
}

void Config::processInstructions(QStringList& instructs, QStringList& copy) {
    instructs.clear();
    // remove invalid moves
    for (int i = 0; i < copy.size(); i++) {
        copy[i] = copy[i].trimmed();
        if (copy[i] == "Left" || copy[i] == "Right" || copy[i] == "Shoot") {
            instructs.append(copy.at(i));
        }
    }
    if (instructs.size() == 0) {
        instructs << defaultInstructs;
    }
}

// Process [SWARM] information
void Config::processSwarm(QTextStream& in) {
    QString type(defaultAlienType);
    QStringList move = defaultAlienMove;
    QList<QPair<int, int>> positions;
    int shoot = defaultAlienShoot;

    while (!in.atEnd()) {
        QString l = in.readLine();
        l = l.trimmed();
        // make some default values...
        if (l.isEmpty()) {
            continue;
        } else if (l.startsWith("position=")) {
            // split by = and then by ,
            l = l.split("=").last();
            QStringList list = l.split(",");
            // now we have a list like this: [10 20] , [30 40]; append if we have nums
            processPairs(list, positions);

        } else if (l.startsWith("type=")) {
            l = l.split("=").last();
            if (alienTypes.contains(l, Qt::CaseSensitive)) {
                type = l;
            }
        } else if (l.startsWith("move=")) {
            l = l.split("=").last();
            QStringList list = l.split(",");
            processMoves(move, list);

        } else if (l.startsWith("shoot=")) {
            // check it's actually an int
            int number = l.split("=").last().toInt();
            if (number != 0) {
                shoot = number;
            }
        } else if (l.startsWith("[SHIP]")) {
            // reads the ship information, saves (possibly incomplete hence default)
            // swarm info
            processShip(in);
            saveSwarm(type, positions, move, shoot);
            return;
        } else if (l.startsWith("[SWARM]")) {
            processSwarm(in);
            saveSwarm(type, positions, move, shoot);
            return;
        } else {
            std::cout << "Incorrect key; check [SWARM] usage" << l.toStdString() << std::endl;
        }
    }

    // reached end of file; SWARM is last object and hasn't saved itself yet
    saveSwarm(type, positions, move, shoot);
}

// Process moves for swarms
void Config::processMoves(QStringList& move, QStringList& list) {
    move.clear();
    // remove anything that isn't L or R
    for (QString c : list) {
        if (c == "L" || c == "R" || c == "U" || c == "D") {
            move.append(c);
        }
    }

    // check move isn't still empty, if so use empty.
    if (move.size() == 0) {
        move.append("L");
    }
}

void Config::processPairs(QStringList list, QList<QPair<int, int>>& positions) {
    for (QString item : list) {
        item = item.trimmed();  // leave internal spaces alone
        QStringList currentPair = item.split(" ");
        // error checking
        if (currentPair.length() != 2) {
            continue;
        }

        // check if both entries are numbers
        int x = currentPair.first().toInt();
        int y = currentPair.last().toInt();
        if (x == 0 || y == 0) {
            continue;
        }

        QPair<int, int> coordinate = qMakePair(x, y);
        positions.append(coordinate);
    }
}

void Config::saveSwarm(
        QString type, QList<QPair<int, int>> positions, QStringList move, int shoot) {
    // only save swarm if there is at least 1 position!
    if (positions.size() > 0) {
        swarmList.append(SwarmInfo(type, positions, move, shoot));
    }
}

// PROCESS CONFIG SIZE CHOICES
void Config::processConfigSizeLine(QString l) {
    // HASHMAP TO ALIGN SHIP SIZES TO SCALES
    QHash<QString, double> Sizes;

    Sizes.insert("Tiny", 0.5);
    Sizes.insert("Normal", 0.8);
    Sizes.insert("Large", 1.2);
    Sizes.insert("Giant", 1.5);

    if (Sizes.contains(l)) {
        scale = Sizes[l];
    } else {
        std::cout << "M8 thats not how you fill out a config file. Size is now default"
                  << std::endl;
        scale = 1;
    }
}

// change config file path to load new level using config<int>.txt
void Config::next_level() {
    level++;
    swarmList = {};
    QString str1 = "../SpaceInvaders/config";
    QString str2 = ".txt";
    QString path = str1 + QString::number(level) + str2;
    QFile c_file(path);
    if (!(c_file.open(QIODevice::ReadOnly))) {
        level = 0;
        next_level();
        return;
    }

    QTextStream in(&c_file);
    initDefault();
    // READ IN CONFIG FOR CUSTOM SETTINGS
    while (!in.atEnd()) {
        // assumes ship first, but it's okay anyway [just read it]
        processShip(in);
    }
    // set the scaled width and height!
    this->SCALEDWIDTH = WIDTH * this->scale;
    this->SCALEDHEIGHT = HEIGHT * this->scale;
    // scales the alien positions based on the scale.
    scalePositions();

}

// Getters
QString Config::get_name() {
    return this->name;
}

double Config::get_scale() {
    return this->scale;
}

QStringList Config::get_instructs() {
    return this->instructs;
}

int Config::get_startpos() {
    return this->startpos;
}
int Config::get_frames() {
    return this->frames;
}

QList<SwarmInfo> Config::getSwarmList() {
    return this->swarmList;
}

int Config::get_SCALEDWIDTH() {
    return this->SCALEDWIDTH;
}

int Config::get_SCALEDHEIGHT() {
    return this->SCALEDHEIGHT;
}

QString Config::get_control() {
    return this->control;
}

void Config::set_level(int level) {
    this->level = level;
}

}  // end namespace
