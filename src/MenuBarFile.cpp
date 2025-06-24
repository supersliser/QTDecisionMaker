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
    newAction->setShortcuts({QKeySequence(Qt::Key::Key_New), QKeySequence(Qt::CTRL | Qt::Key::Key_N), QKeySequence::New});
    openAction->setShortcuts({QKeySequence(Qt::Key::Key_Open), QKeySequence(Qt::CTRL | Qt::Key::Key_O), QKeySequence::Open});
    saveAction->setShortcuts({QKeySequence(Qt::Key::Key_Save), QKeySequence(Qt::CTRL | Qt::Key::Key_S), QKeySequence::Save});
    saveAsAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key::Key_Shift | Qt::Key::Key_S), QKeySequence(Qt::CTRL | Qt::Key::Key_S)});


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

void MenuBarFile::_new(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the new action
    emit newClicked();
}

void MenuBarFile::_open(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the open action
    emit openClicked();
}

void MenuBarFile::_save(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the save action
    emit saveClicked();
}

void MenuBarFile::_saveAs(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the save as action
    emit saveAsClicked();
}

void MenuBarFile::_close(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the close action
    emit closeClicked();
}

void MenuBarFile::_quit(bool i_checked)
{
    Q_UNUSED(i_checked);
    // Handle the quit action
    emit quitClicked();
}
