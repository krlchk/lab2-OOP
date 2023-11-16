#include "notewindow.h"
#include <fstream>

#include <QDebug>
#include "note.h"
#include "task.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoteWindow w;
    w.show();
    return a.exec();
}
