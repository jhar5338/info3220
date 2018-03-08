#include "gamedialog.h"
#include "bullet.h"
#include "ship.h"
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QPixmap>
#include <QSound>
#include <QTimer>
#include <QWidget>
#include <vector>
#include <iostream>

namespace game {

GameDialog::GameDialog(QWidget* parent)
        : QDialog(parent), bullets(), shipFiringSound(this), gameScore(0) {
    // SET UP GAME DIMENSIONS AND CONFIG
    Config* c = Config::getInstance();
    SCALEDWIDTH = c->get_SCALEDWIDTH();
    SCALEDHEIGHT = c->get_SCALEDHEIGHT();
    this->frames = c->get_frames();
    setMouseTracking(true);
    control = c->get_control();
    // MENU
    QList<QPair<QString, int>> dummy;
    menu = new Menu(this, c->get_name(), this->gameScore, dummy, SCALEDWIDTH);

    // EXTENSION STAGE 1 PART 1 - RESCALE GAME SCREEN FOR SHIP SIZE
    this->setFixedWidth(SCALEDWIDTH);
    this->setFixedHeight(SCALEDHEIGHT);
    // SHIP
    QPixmap ship;
    ship.load(":/Images/ship.png");
    this->ship = new Ship(ship, c->get_scale(), c->get_startpos(), SCALEDHEIGHT);
    this->next_instruct = 0;
    // SHIP SOUND
    shipFiringSound.setSource(QUrl::fromLocalFile(":/Sounds/shoot.wav"));
    shipFiringSound.setVolume(0.3f);

    // ALIENS
    generateAliens(c->getSwarmList());

    // SET BACKGROUND
    setStyleSheet("background-color: #000000;");

    paused = false;
    dead = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start(this->frames);

    update();
}

GameDialog::~GameDialog() {
    delete ship;
    delete timer;  // optional, don't have to do this apparently

    // loop though swarms to delete aliens
    delete swarms;  // recursively deletes itself.
    for (Bullet* b : bullets) {
        delete b;
    }
}

// make the swarms for this level.
void GameDialog::generateAliens(const QList<SwarmInfo>& makeSwarms) {
    SwarmInfo def = SwarmInfo();

    swarms = new Swarm(def, *this->ship);
    for (SwarmInfo info : makeSwarms) {
        AlienBase* s = new Swarm(info, *this->ship);
        swarms->add(s);
    }
}

void GameDialog::pauseStart() {
    if (this->paused) {
        // start game
        this->paused = false;
        this->menu->displayMenu(paused,false);
        this->timer->start(frames);
    } else {
        this->paused = true;
        this->menu->displayMenu(paused,false);
        this->timer->stop();
    }
}

void GameDialog::keyPressEvent(QKeyEvent* event) {

    if (event->key() == Qt::Key_P||event->key() == Qt::Key_Escape) {
        if (!dead) {
            pauseStart();
        }
    } else if (event->key() == Qt::Key_Equal) {
        if (frames > 3) {
            frames = frames - 3;
            this->timer->start(frames);
        }
    } else if (event->key() == Qt::Key_Minus) {
        if (frames < 100) {
            frames += 3;
            this->timer->start(frames);
        }
    }
    if (control=="keyboard") {
        if (event->key() == Qt::Key_Left) {
            ins = "Left";
        } else if (event->key() == Qt::Key_Right) {
            ins = "Right";
        } else if (event->key() == Qt::Key_Space) {
            ins = "Shoot";
        }
    }
}

void GameDialog::keyReleaseEvent(QKeyEvent *event) {
    if (control=="keyboard") {
        if (event->key() == Qt::Key_Left) {
            ins = "";
        } else if (event->key() == Qt::Key_Right) {
            ins = "";
        }
    }
}

void GameDialog::mouseMoveEvent(QMouseEvent *event) {
    if (control=="mouse")
        ship->set_x(event->x()-(ship->get_image().width()/2));

}

void GameDialog::mousePressEvent(QMouseEvent *event) {
    if (control=="mouse")
        ins = "Shoot";
}

// FOLLOWING EACH INSTRUCTION TO FRAME - for PLAYER ship.
void GameDialog::nextFrame() {

    if (!paused) {
        Config *c = Config::getInstance();

        if (!QString::compare(c->get_control(),"config")) {
            QStringList instruct = c->get_instructs();
            if (next_instruct >= instruct.size()) {
                next_instruct = next_instruct % instruct.size();
            }
            ins = instruct[next_instruct];
            next_instruct++;
        }
        if (ins == "Left") {
            ship->move_left();

        } else if (ins == "Right") {
            ship->move_right();

        } else if (ins == "Shoot") {
            std::vector<Bullet*> temp;
            // super mode ship if more than 3 aliens left
            if (swarms->getAliens().at(0)->getAliens().size() > 3) {
                temp = this->ship->shoot(true);
            } else {
                temp = this->ship->shoot(false);
            }
            if (temp.size()==1) {
                bullets.push_back(temp.at(0));
            } else if (temp.size()==3) {
                bullets.push_back(temp.at(0));
                temp.at(1)->set_x(temp.at(0)->get_x()-10);
                temp.at(2)->set_x(temp.at(0)->get_x()+10);
                bullets.push_back(temp.at(1));
                bullets.push_back(temp.at(2));
            }
            this->shipFiringSound.play();
            ins = "";
        }

        updateBullets();

        // loop through each alien swarm, move and calculated collisions
        swarms->move("");  // recursive.
        checkSwarmCollisions(swarms);
        addBullets(swarms->shoot(""));
    }
    // prepare collisions and calculate score
    update();
}

void GameDialog::paintBullets(QPainter& painter) {
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        painter.drawPixmap(b->get_x(), b->get_y(), b->get_image());
    }
}

void GameDialog::updateBullets()
{
    for (int i = 0; i < bullets.size(); i++) {
        Bullet* b = bullets[i];
        // WHEN BULLET OFF GAME SCREEN, FREE MEMORY AND DELETE
        int score = get_collided(b, swarms);
        if (b->get_y() < 0 || b->get_y() >= SCALEDHEIGHT || b->get_x() < 0 ||
                b->get_x() >= SCALEDWIDTH || score > 0) {
            delete b;
            bullets.erase(bullets.begin() + i);
            i--;
        } else if (score == -1) {
            this->paused = true;
            this->menu->displayMenu(paused,true);
            this->timer->stop();
            this->dead = true;
            Config* c = Config::getInstance();
            this->menu->addScore(c->get_name(),gameScore);

        } else
        {
            b->move();// we move at the end so that we can see collisions before the game ends
        }
        gameScore += score;


    }
}

// recurse throughout the tree and draw everything.
// will also
void GameDialog::paintSwarm(QPainter& painter, AlienBase*& root) {
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), draw it.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            painter.drawPixmap(child->get_x(), child->get_y(), child->get_image());
        } else {
            paintSwarm(painter, child);
        }
    }
}

//check if any aliens based off the alien root are crashing with the player ship.
void GameDialog::checkSwarmCollisions(AlienBase *&root)
{
    for (AlienBase* child : root->getAliens()) {
        // if the child is a leaf (i.e., an alien that has no children), check for collisions.
        const QList<AlienBase*>& list = child->getAliens();
        if (list.size() == 0) {  // leaf
            // check if it is crashing into the player ship
            if (child->collides(*this->ship)) {
                this->paused = true;
                this->menu->displayMenu(paused,true);
                this->timer->stop();
                this->dead = true;
                Config* c = Config::getInstance();
                this->menu->addScore(c->get_name(),gameScore);
            }
        } else {
          checkSwarmCollisions(child);
        }
    }
}

// PAINTING THE SHIP AND ANY BULLETS
void GameDialog::paintEvent(QPaintEvent*) {
    // SHIP
    QPainter painter(this);

    painter.drawPixmap(ship->get_x(), ship->get_y(), ship->get_image());

    // loop through each alien swarm and draw
    paintSwarm(painter, swarms);

    // BULLETS last so they draw over everything
    paintBullets(painter);

    // check if last alien killed to start new level
    if (swarms->getAliens().size() == 0) {
        Config* c = Config::getInstance();
        c->next_level();
        refresh();
    }
}

// if this bullet is unfriendly, only check if it hits Ship
// if this bullet is friendly, will check the swarm;
// returns the score from the deleted hit object.
// Returns 0 if nothing hit, -ve if ship is hit.
int GameDialog::get_collided(Bullet*& b, AlienBase*& root) {

    int totalScore = 0;

    // if it's an enemy bullet, then don't look at the swarm.
    if (!b->isFriendly()) {
        // check it hits the player ship
        if (b->collides(*this->ship)) {
            totalScore = -1;

        }  // future; add barriers here.
    } else {
        totalScore += get_collided_swarm(b, root);
    }
    return totalScore;
}

// helper function, recursively searches swarms.
int GameDialog::get_collided_swarm(Bullet*& b, AlienBase*& root) {
    int totalScore = 0;
    // Case 1: you are a leaf node, or root node
    if (root->getAliens().size() == 0) {
        // check collision.
        if (b->collides(*root)) {
            // if it is an alien, it will return >0.
            // if it's a swarm (i.e., root) it will return 0.
            return root->get_score();
        }

    } else {
        for (int i = 0; i < root->getAliens().size(); i++) {
            AlienBase* child = root->getAliens().at(i);
            // if it's another swarm, recurse down the tree
            totalScore += get_collided_swarm(b, child);
            // if something was hit, score > 0
            if (totalScore > 0 && child->getAliens().size() == 0) {
                // some children shoot more bullets when they die.
                // ask child for reaction when you're going to delete them
                addBullets(child->react());
                root->remove(child);
                i--;
                return totalScore;
            }
        }
    }
    return totalScore;
}

void GameDialog::addBullets(const QList<Bullet*>& list) {
    for (Bullet* b : list) {
        this->bullets.push_back(b);
    }
}

// refresh the game
void GameDialog::refresh() {

    this->paused = false;
    menu->displayMenu(paused,false);
    this->timer->start(frames);
    bullets.clear();
    Config* c = Config::getInstance();
    control = c->get_control();
    this->ship->set_x(c->get_startpos());
    this->next_instruct = 0;
    ins = "";

    delete swarms;
    generateAliens(c->getSwarmList());

    // ensures controls work after refresh
    this->setFocusPolicy(Qt::StrongFocus);
}

// restart the game
void GameDialog::restart() {

    this->paused = false;
    this->dead = false;
    menu->displayMenu(paused,false);
    this->timer->start(frames);
    bullets.clear();
    Config* c = Config::getInstance();
    control = c->get_control();
    this->ship->set_x(c->get_startpos());
    this->next_instruct = 0;
    this->gameScore = 0;
    c->set_level(0);
    c->next_level();
    ins = "";

    delete swarms;
    generateAliens(c->getSwarmList());

    // ensures controls work after restart
    this->setFocusPolicy(Qt::StrongFocus);
}

// called when settings button pressed to open settings
void GameDialog::settings() {
    menu->settingsMenu();
}

// change game speed to slow
void GameDialog::slow() {
    frames = 70;
    this->timer->start(frames);
    this->setFocusPolicy(Qt::StrongFocus);
}

// change game speed to medium
void GameDialog::med() {
    frames = 55;
    this->timer->start(frames);
    this->setFocusPolicy(Qt::StrongFocus);
}

// change game speed to fast
void GameDialog::fast() {
    frames = 40;
    this->timer->start(frames);
    this->setFocusPolicy(Qt::StrongFocus);
}

// change background to black
void GameDialog::black() {
    setStyleSheet("background-color: #000000;");
    this->setFocusPolicy(Qt::StrongFocus);
}

// change background to white
void GameDialog::white() {
    setStyleSheet("background-color: #FFFFFF;");
    this->setFocusPolicy(Qt::StrongFocus);
}
}
