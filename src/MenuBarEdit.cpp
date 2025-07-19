//
// Created by t on 24/06/25.
//

#include "../include/MenuBarEdit.h"
#include <QAction>
#include <QMenu>

#include "TableViewerWindow.h"

MenuBarEdit::MenuBarEdit(QWidget* parent)
    : QMenu(parent)
{
    // Set the title of the menu
    setTitle(tr("Edit"));

    // Create actions for edit operations
    auto* undoAction = new QAction(tr("Undo"), this);
    auto* redoAction = new QAction(tr("Redo"), this);
    auto* cutAction = new QAction(tr("Cut"), this);
    auto* copyAction = new QAction(tr("Copy"), this);
    auto* pasteAction = new QAction(tr("Paste"), this);
    auto* preferencesAction = new QAction(tr("Preferences"), this);
    auto* findAction = new QAction(tr("Find"), this);

    // Connect actions to slots
    connect(undoAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::undoTriggered);
    connect(redoAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::redoTriggered);
    connect(cutAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::cutTriggered);
    connect(copyAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::copyTriggered);
    connect(pasteAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::pasteTriggered);
    connect(preferencesAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::preferencesTriggered);
    connect(findAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::findTriggered);

    // Add keybinds
    undoAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Z)});
    redoAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Y)});
    cutAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_X)});
    copyAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_C)});
    pasteAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_V)});
    findAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_F)});

    // Add actions to the menu
    addAction(undoAction);
    addAction(redoAction);
    addSeparator();
    addAction(cutAction);
    addAction(copyAction);
    addAction(pasteAction);
    addSeparator();
    addAction(findAction);
    addSeparator();
    addAction(preferencesAction);
}
