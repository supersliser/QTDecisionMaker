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
    connect(_m_viewMenu, &MenuBarView::filterChanged, this, &TableViewerMenubar::_filterChanged);
    connect(_m_viewMenu, &MenuBarView::sortChanged, this, &TableViewerMenubar::_sortChanged);
    connect(_m_editMenu, &MenuBarEdit::reorderColumns, this, &TableViewerMenubar::_reorderColumns);
    connect(_m_editMenu, &MenuBarEdit::reorderRows, this, &TableViewerMenubar::_reorderRows);
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

void TableViewerMenubar::_filterChanged(const QString& filterText)
{
    emit filterChanged(filterText);
}

void TableViewerMenubar::_sortChanged(int columnIndex, bool ascending)
{
    emit sortChanged(columnIndex, ascending);
}

void TableViewerMenubar::_reorderColumns()
{
    emit reorderColumns();
}

void TableViewerMenubar::_reorderRows()
{
    emit reorderRows();
}
