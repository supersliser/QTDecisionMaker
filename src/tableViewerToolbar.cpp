//
// Created by t on 14/05/25.
//

#include "../include/tableViewerToolbar.h"

#include <QAction>
#include <QToolButton>
#include <QIcon>
#include <QHBoxLayout>
#include <QDebug>
#include <QPushButton>
#include <QStyle>

void tableViewerToolbar::_newRow(bool checked)
{
    emit newRow();
}

void tableViewerToolbar::_newColumn(bool checked)
{
    emit newColumn();
}

tableViewerToolbar::tableViewerToolbar(QWidget* parent)
    : QToolBar(parent)
{
    // Create actions for adding rows and columns
    QAction* addRowAction = new QAction(tr("Add Row"), this);
    QAction* addColumnAction = new QAction(tr("Add Column"), this);

    // Connect actions to signals
    connect(addRowAction, &QAction::triggered, this, &tableViewerToolbar::_newRow);
    connect(addColumnAction, &QAction::triggered, this, &tableViewerToolbar::_newColumn);

    // Add actions to the toolbar
    addAction(addRowAction);
    addAction(addColumnAction);
}