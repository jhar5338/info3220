#include "gamedialog.h"
#include <QApplication>
using namespace game;

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    GameDialog w;
    w.show();

    return a.exec();
}
