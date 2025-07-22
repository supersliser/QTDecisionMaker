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
    _m_editMenu = new MenuBarEdit(this);
    _m_viewMenu = new MenuBarView(this);
    _m_helpMenu = new MenuBarHelp(this);
    connect(_m_viewMenu, &MenuBarView::zoom, this, &TableViewerMenubar::_zoom);
    addMenu(_m_fileMenu);
    addMenu(_m_editMenu);
    addMenu(_m_viewMenu);
    addMenu(_m_helpMenu);
    dynamic_cast<TableViewerWindow*>(parent)->setMenuBar(this);
}

void TableViewerMenubar::_zoom(float i_newZoomAmount)
{
    emit zoom(i_newZoomAmount);
}
