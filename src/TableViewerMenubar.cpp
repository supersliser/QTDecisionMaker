//
// Created by t on 14/05/25.
//

#include "../include/TableViewerMenubar.h"

#include "TableViewerWindow.h"

TableViewerMenubar::TableViewerMenubar(QWidget* parent)
    : QMenuBar(parent)
{
    // Create the File menu
    _m_fileMenu = new MenuBarFile(this);
    addMenu(_m_fileMenu);
    dynamic_cast<TableViewerWindow*>(parent)->setMenuBar(this);
}
