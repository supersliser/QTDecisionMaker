//
// Created by copilot for issue #24
//

#include "../include/MenuBarHelp.h"
#include <QAction>
#include <QMenu>

#include "TableViewerWindow.h"

MenuBarHelp::MenuBarHelp(QWidget* parent)
    : QMenu(parent)
{
    // Set the title of the menu
    setTitle(tr("Help"));

    // Create actions for help operations
    auto* searchAction = new QAction(tr("Search"), this);
    auto* reportBugAction = new QAction(tr("Report a bug"), this);
    auto* viewSourceAction = new QAction(tr("View Source"), this);
    auto* openForumsAction = new QAction(tr("Open Forums"), this);
    auto* documentationAction = new QAction(tr("Documentation"), this);

    // Connect actions to slots
    connect(searchAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::searchTriggered);
    connect(reportBugAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::reportBugTriggered);
    connect(viewSourceAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::viewSourceTriggered);
    connect(openForumsAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::openForumsTriggered);
    connect(documentationAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::documentationTriggered);

    // Add actions to the menu
    addAction(searchAction);
    addSeparator();
    addAction(reportBugAction);
    addSeparator();
    addAction(viewSourceAction);
    addSeparator();
    addAction(openForumsAction);
    addSeparator();
    addAction(documentationAction);
}