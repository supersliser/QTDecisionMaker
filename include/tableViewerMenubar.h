//
// Created by t on 14/05/25.
//

#ifndef TABLEVIEWERMENUBAR_H
#define TABLEVIEWERMENUBAR_H
#include <QMenuBar>
#include "MenuBarFile.h"


class tableViewerMenubar: public QMenuBar
{
    Q_OBJECT

public:
    tableViewerMenubar(QWidget* parent = nullptr);
    ~tableViewerMenubar() override = default;
};



#endif //TABLEVIEWERMENUBAR_H
