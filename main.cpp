#include "tableviewerwindow.h"

#include <QApplication>
#include <QMenuBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TableViewerWindow w;
    w.menuBar()->setVisible(true);
    w.show();
    return a.exec();
}
