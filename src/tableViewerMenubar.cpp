//
// Created by t on 14/05/25.
//

#include "../include/tableViewerMenubar.h"

#include "tableviewerwindow.h"

tableViewerMenubar::tableViewerMenubar(QWidget* parent)
    : QMenuBar(parent)
{
    // Create the File menu
    fileMenu = new MenuBarFile(this);
    addMenu(fileMenu);
    dynamic_cast<TableViewerWindow*>(parent)->setMenuBar(this);
}
