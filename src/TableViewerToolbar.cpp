//
// Created by t on 14/05/25.
//

#include "../include/TableViewerToolbar.h"

#include <QAction>
#include <QToolButton>
#include <QIcon>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QStyle>

void TableViewerToolbar::_newRow(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit newRow();
}

void TableViewerToolbar::_newColumn(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit newColumn();
}

TableViewerToolbar::TableViewerToolbar(QWidget* parent)
    : QToolBar(parent)
{
    // Create actions for adding rows and columns
    QAction* addRowAction = new QAction(tr("Add Row"), this);
    QAction* addColumnAction = new QAction(tr("Add Column"), this);

    // Connect actions to signals
    connect(addRowAction, &QAction::triggered, this, &TableViewerToolbar::_newRow);
    connect(addColumnAction, &QAction::triggered, this, &TableViewerToolbar::_newColumn);

    // Add actions to the toolbar
    addAction(addRowAction);
    addAction(addColumnAction);
}