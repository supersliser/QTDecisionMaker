//
// Created by t on 14/05/25.
//

#include "../include/MenuBarFile.h"

#include <QAction>

#include <QMenu>

#include "tableviewerwindow.h"

MenuBarFile::MenuBarFile(QWidget* parent)
    : QMenu(parent)
{


    // Set the title of the menu
    setTitle(tr("File"));

    // Create actions for file operations
    QAction* newAction = new QAction(tr("New"), this);
    QAction* openAction = new QAction(tr("Open"), this);
    QAction* saveAction = new QAction(tr("Save"), this);
    QAction* saveAsAction = new QAction(tr("Save As"), this);
    QAction* closeAction = new QAction(tr("Close"), this);
    QAction* quitAction = new QAction(tr("Quit"), this);

//     Connect actions to slots
     connect(newAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::newTriggered);
     connect(openAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::openTriggered);
     connect(saveAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveTriggered);
     connect(saveAsAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveAsTriggered);
     connect(closeAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::closeTriggered);
     connect(quitAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::quitTriggered);

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

void MenuBarFile::_new(bool checked)
{
    Q_UNUSED(checked);
    // Handle the new action
    emit newClicked();
}

void MenuBarFile::_open(bool checked)
{
    Q_UNUSED(checked);
    // Handle the open action
    emit openClicked();
}

void MenuBarFile::_save(bool checked)
{
    Q_UNUSED(checked);
    // Handle the save action
    emit saveClicked();
}

void MenuBarFile::_saveAs(bool checked)
{
    Q_UNUSED(checked);
    // Handle the save as action
    emit saveAsClicked();
}

void MenuBarFile::_close(bool checked)
{
    Q_UNUSED(checked);
    // Handle the close action
    emit closeClicked();
}

void MenuBarFile::_quit(bool checked)
{
    Q_UNUSED(checked);
    // Handle the quit action
    emit quitClicked();
}
