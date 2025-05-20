//
// Created by t on 14/05/25.
//

#ifndef TABLEVIEWERMENUBAR_H
#define TABLEVIEWERMENUBAR_H

#include <QMenuBar>
#include "MenuBarFile.h"


class TableViewerMenubar : public QMenuBar {
Q_OBJECT

public:
    TableViewerMenubar(QWidget *parent = nullptr);

    ~TableViewerMenubar() override = default;

signals:

    void newClicked();

    void openClicked();

    void saveClicked();

    void saveAsClicked();

    void closeClicked();

    void quitClicked();

private:
    MenuBarFile *_m_fileMenu;
};


#endif //TABLEVIEWERMENUBAR_H
