//
// Created by t on 14/05/25.
//

#include "../include/TableViewerMenubar.h"

#include "TableViewerWindow.h"
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>

TableViewerMenubar::TableViewerMenubar(QWidget* parent)
    : QMenuBar(parent)
{
    // Create the File menu
    _m_fileMenu = new MenuBarFile(this);
    _m_editMenu = new MenuBarEdit(this);
    _m_viewMenu = new MenuBarView(this);
    _m_helpMenu = new MenuBarHelp(this);
    
    // Create Filter menu
    _m_filterMenu = new QMenu(tr("Filter"), this);
    auto filterAction = new QAction(tr("Filter..."), this);
    _m_filterMenu->addAction(filterAction);
    connect(filterAction, &QAction::triggered, this, &TableViewerMenubar::_showFilterDialog);
    
    // Create Sort menu
    _m_sortMenu = new QMenu(tr("Sort"), this);
    auto sortAction = new QAction(tr("Sort..."), this);
    _m_sortMenu->addAction(sortAction);
    connect(sortAction, &QAction::triggered, this, &TableViewerMenubar::_showSortDialog);
    
    connect(_m_viewMenu, &MenuBarView::zoom, this, &TableViewerMenubar::_zoom);
    
    addMenu(_m_fileMenu);
    addMenu(_m_editMenu);
    addMenu(_m_viewMenu);
    addMenu(_m_filterMenu);
    addMenu(_m_sortMenu);
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

void TableViewerMenubar::_showFilterDialog()
{
    bool ok;
    QString filterText = QInputDialog::getText(this, tr("Filter Table"), 
                                             tr("Enter filter text:"), 
                                             QLineEdit::Normal, 
                                             QString(), &ok);
    if (ok) {
        emit filterChanged(filterText);
    }
}

void TableViewerMenubar::_showSortDialog()
{
    // Get the table window to access column information
    TableViewerWindow* mainWindow = qobject_cast<TableViewerWindow*>(this->parent());
    
    if (!mainWindow) {
        QMessageBox::warning(this, tr("Sort"), tr("Cannot access table data."));
        return;
    }

    // Simple sort dialog with basic column options
    QStringList items;
    items << tr("Name Column") << tr("Total Value");
    // Add a few generic column options
    for (int i = 1; i <= 5; i++) {
        items << tr("Column %1").arg(i);
    }
    
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Sort Table"),
                                       tr("Select column to sort by:"), items, 0, false, &ok);
    if (ok && !item.isEmpty()) {
        int columnIndex = items.indexOf(item);
        
        // Ask for sort direction
        QStringList directions;
        directions << tr("Ascending") << tr("Descending");
        QString direction = QInputDialog::getItem(this, tr("Sort Direction"),
                                                tr("Select sort direction:"), directions, 0, false, &ok);
        if (ok) {
            bool ascending = (direction == tr("Ascending"));
            emit sortChanged(columnIndex, ascending);
        }
    }
}
