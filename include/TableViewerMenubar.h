//
// Created by t on 14/05/25.
//

#ifndef TABLEVIEWERMENUBAR_H
#define TABLEVIEWERMENUBAR_H

#include <QMenuBar>

#include "MenuBarEdit.h"
#include "MenuBarFile.h"


class TableViewerMenubar : public QMenuBar {
Q_OBJECT

public:
    TableViewerMenubar(QWidget *parent = nullptr);

    ~TableViewerMenubar() override = default;

private:
    MenuBarFile *_m_fileMenu;
    MenuBarEdit *_m_editMenu;
};


#endif //TABLEVIEWERMENUBAR_H
