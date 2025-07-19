//
// Created by t on 14/05/25.
//

#include "../include/MenuBarFile.h"

#include <QAction>

#include <QMenu>

#include "TableViewerWindow.h"

MenuBarFile::MenuBarFile(QWidget* parent)
    : QMenu(parent)
{
    // Set the title of the menu
    setTitle(tr("File"));

    // Create actions for file operations
    auto* newAction = new QAction(tr("New"), this);
    auto* openAction = new QAction(tr("Open"), this);
    auto* saveAction = new QAction(tr("Save"), this);
    auto* saveAsAction = new QAction(tr("Save As"), this);
    auto* closeAction = new QAction(tr("Close"), this);
    auto* quitAction = new QAction(tr("Quit"), this);

//     Connect actions to slots
     connect(newAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::newTriggered);
     connect(openAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::openTriggered);
     connect(saveAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveTriggered);
     connect(saveAsAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveAsTriggered);
     connect(closeAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::closeTriggered);
     connect(quitAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::quitTriggered);

    // add keybinds
    newAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key::Key_N)});
    openAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key::Key_O)});
    saveAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key::Key_S)});
    saveAsAction->setShortcuts({QKeySequence(Qt::CTRL | (Qt::Key::Key_Shift | Qt::Key::Key_S))});

    // Add actions to the menu
    addAction(newAction);
    addAction(openAction);
    addSeparator();
    addAction(saveAction);
    addAction(saveAsAction);
    addSeparator();
    addAction(closeAction);
    addAction(quitAction);
}