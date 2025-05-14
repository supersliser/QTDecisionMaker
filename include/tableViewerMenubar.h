//
// Created by t on 14/05/25.
//

#ifndef TABLEVIEWERMENUBAR_H
#define TABLEVIEWERMENUBAR_H

#include <QMenuBar>
#include "MenuBarFile.h"


class tableViewerMenubar : public QMenuBar {
Q_OBJECT

public:
    tableViewerMenubar(QWidget *parent = nullptr);

    ~tableViewerMenubar() override = default;

signals:

    void newClicked();

    void openClicked();

    void saveClicked();

    void saveAsClicked();

    void closeClicked();

    void quitClicked();

private:
    MenuBarFile *fileMenu;
    // Add other menus as needed
    // QMenu* editMenu;
    // QMenu* viewMenu;
    // QMenu* helpMenu;
};


#endif //TABLEVIEWERMENUBAR_H
