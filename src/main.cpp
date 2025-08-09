#include <QApplication>
#include "Genshin_Midi_Auto_Player.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/appicon.ico"));
    GenshinMidiAutoPlayer window;
    window.show();
    return app.exec();
}