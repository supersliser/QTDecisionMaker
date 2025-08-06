#include "../include/MenuBarView.h"
#include <QAction>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

#include "TableViewerWindow.h"

MenuBarView::MenuBarView(QWidget* parent)
    : QMenu(parent)
{
    setTitle(tr("View"));

    auto zoom25Action = new QAction(tr("20%"), this);
    auto zoom50Action = new QAction(tr("50%"), this);
    auto zoom100Action = new QAction(tr("100%"), this);
    auto zoom200Action = new QAction(tr("200%"), this);

    auto zoomIncreaseAction = new QAction(tr("Increase"), this);
    auto zoomDecreaseAction = new QAction(tr("Decrease"), this);

    connect(zoom25Action, &QAction::triggered, this, &MenuBarView::_zoom25);
    connect(zoom50Action, &QAction::triggered, this, &MenuBarView::_zoom50);
    connect(zoom100Action, &QAction::triggered, this, &MenuBarView::_zoom100);
    connect(zoom200Action, &QAction::triggered, this, &MenuBarView::_zoom200);
    connect(zoomIncreaseAction, &QAction::triggered, this, &MenuBarView::_zoomin);
    connect(zoomDecreaseAction, &QAction::triggered, this, &MenuBarView::_zoomout);

    zoomIncreaseAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Plus)});
    zoomDecreaseAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Minus)});

    auto zoomMenu = new QMenu(this);
    zoomMenu->addAction(zoom25Action);
    zoomMenu->addAction(zoom50Action);
    zoomMenu->addAction(zoom100Action);
    zoomMenu->addAction(zoom200Action);
    zoomMenu->setTitle("Zoom");

    // Filter and Sort actions
    auto filterAction = new QAction(tr("Filter..."), this);
    auto sortAction = new QAction(tr("Sort..."), this);
    
    connect(filterAction, &QAction::triggered, this, &MenuBarView::_showFilterDialog);
    connect(sortAction, &QAction::triggered, this, &MenuBarView::_showSortDialog);

    addMenu(zoomMenu);
    addAction(zoomIncreaseAction);
    addAction(zoomDecreaseAction);
    addSeparator();
    addAction(filterAction);
    addAction(sortAction);
}

void MenuBarView::_zoom25()
{
    _m_currentZoom = 25;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_zoom50()
{
    _m_currentZoom = 50;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_zoom100()
{
    _m_currentZoom = 100;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_zoom200()
{
    _m_currentZoom = 200;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_zoomin()
{
    _m_currentZoom = _m_currentZoom += 25;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_zoomout()
{
    _m_currentZoom = _m_currentZoom -= 25;
    emit zoom(_m_currentZoom);
}

void MenuBarView::_showFilterDialog()
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

void MenuBarView::_showSortDialog()
{
    // Get the table window to access column information
    TableViewerWindow* mainWindow = nullptr;
    QWidget* currentParent = this->parentWidget();
    while (currentParent) {
        mainWindow = qobject_cast<TableViewerWindow*>(currentParent);
        if (mainWindow) break;
        currentParent = currentParent->parentWidget();
    }
    
    if (!mainWindow) {
        QMessageBox::warning(this, tr("Sort"), tr("Cannot access table data."));
        return;
    }

    // Simple sort dialog - this could be improved with a proper dialog
    QStringList items;
    items << tr("Name Column");
    // Add more column options based on table data
    items << tr("Column 1") << tr("Column 2") << tr("Total Value");
    
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