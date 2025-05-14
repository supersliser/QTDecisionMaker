//
// Created by t on 14/05/25.
//

#include "../include/tableViewerMenubar.h"

tableViewerMenubar::tableViewerMenubar(QWidget* parent)
    : QMenuBar(parent)
{
    // Create the File menu
    fileMenu = new MenuBarFile(this);
    connect(fileMenu, &MenuBarFile::newClicked, this, &tableViewerMenubar::newClicked);

    addMenu(fileMenu);
}