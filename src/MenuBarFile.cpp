//
// Created by t on 14/05/25.
//

#include "../include/MenuBarFile.h"
#include "../include/PreferencesManager.h"

#include <QAction>
#include <QMenu>
#include <QFileInfo>

#include "TableViewerWindow.h"

MenuBarFile::MenuBarFile(QWidget* parent)
    : QMenu(parent)
    , _preferencesManager(nullptr)
    , _recentFilesMenu(nullptr)
    , _recentFilesSeparator(nullptr)
{
    // Set the title of the menu
    setTitle(tr("File"));

    // Create actions for file operations
    auto* newAction = new QAction(tr("New"), this);
    auto* openAction = new QAction(tr("Open"), this);
    auto* saveAction = new QAction(tr("Save"), this);
    auto* saveAsAction = new QAction(tr("Save As"), this);
    auto* preferencesAction = new QAction(tr("Preferences"), this);
    auto* closeAction = new QAction(tr("Close"), this);
    auto* quitAction = new QAction(tr("Quit"), this);

    //     Connect actions to slots
    connect(newAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::newTriggered);
    connect(openAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::openTriggered);
    connect(saveAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveTriggered);
    connect(saveAsAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::saveAsTriggered);
    connect(preferencesAction, &QAction::triggered, dynamic_cast<TableViewerWindow*>(parent->parent()), &TableViewerWindow::preferencesTriggered);
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
    
    // Recent files menu will be added here when preferences manager is set
    setupRecentFilesMenu();
    
    addSeparator();
    addAction(saveAction);
    addAction(saveAsAction);
    addSeparator();
    addAction(preferencesAction);
    addSeparator();
    addAction(closeAction);
    addAction(quitAction);
}

void MenuBarFile::setPreferencesManager(PreferencesManager* prefsManager)
{
    _preferencesManager = prefsManager;
    updateRecentFiles();
}

void MenuBarFile::setupRecentFilesMenu()
{
    _recentFilesMenu = new QMenu(tr("Recent Files"), this);
    _recentFilesSeparator = addMenu( _recentFilesMenu); // Insert after "Open"
    
    // Initialize with empty state
    auto* noRecentAction = new QAction(tr("No recent files"), _recentFilesMenu);
    noRecentAction->setEnabled(false);
    _recentFilesMenu->addAction(noRecentAction);
}

void MenuBarFile::updateRecentFiles()
{
    if (!_preferencesManager || !_recentFilesMenu)
        return;
    
    // Clear existing actions
    _recentFilesMenu->clear();
    _recentFileActions.clear();
    
    QStringList recentFiles = _preferencesManager->getRecentFiles();
    
    if (recentFiles.isEmpty())
    {
        auto* noRecentAction = new QAction(tr("No recent files"), _recentFilesMenu);
        noRecentAction->setEnabled(false);
        _recentFilesMenu->addAction(noRecentAction);
    }
    else
    {
        for (int i = 0; i < recentFiles.size() && i < MAX_RECENT_FILES; ++i)
        {
            QString filePath = recentFiles.at(i);
            QFileInfo fileInfo(filePath);
            QString actionText = QString("&%1 %2").arg(i + 1).arg(fileInfo.fileName());
            
            auto* action = new QAction(actionText, _recentFilesMenu);
            action->setData(filePath);
            action->setToolTip(filePath);
            
            connect(action, &QAction::triggered, this, &MenuBarFile::openRecentFile);
            
            _recentFilesMenu->addAction(action);
            _recentFileActions.append(action);
        }
        
        // Add separator and clear action
        _recentFilesMenu->addSeparator();
        auto* clearAction = new QAction(tr("Clear Recent Files"), _recentFilesMenu);
        connect(clearAction, &QAction::triggered, [this]() {
            if (_preferencesManager) {
                _preferencesManager->clearRecentFiles();
                updateRecentFiles();
            }
        });
        _recentFilesMenu->addAction(clearAction);
    }
}

void MenuBarFile::openRecentFile()
{
    auto* action = qobject_cast<QAction*>(sender());
    if (action)
    {
        QString filePath = action->data().toString();
        // Trigger open with the specific file path
        auto* window = dynamic_cast<TableViewerWindow*>(parent()->parent());
        if (window)
        {
            window->openRecentFile(filePath);
        }
    }
}