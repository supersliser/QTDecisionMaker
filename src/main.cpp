#include "TableViewerWindow.h"

#include <QApplication>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TableViewerWindow w;
    w.show();
    return a.exec();
}
