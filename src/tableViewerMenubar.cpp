//
// Created by t on 14/05/25.
//

#include "../include/tableViewerMenubar.h"

tableViewerMenubar::tableViewerMenubar(QWidget* parent)
    : QMenuBar(parent)
{
    // Create the File menu
    MenuBarFile* fileMenu = new MenuBarFile(this);
    addMenu(fileMenu);

    // Add other menus as needed
}