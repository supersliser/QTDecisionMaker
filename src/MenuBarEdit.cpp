//
// Created by t on 24/06/25.
//

#include "../include/MenuBarEdit.h"
#include <QAction>
#include <QMenu>

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
    auto* selectAllAction = new QAction(tr("Select All"), this);
    auto* preferencesAction = new QAction(tr("Preferences"), this);
    auto* findAction = new QAction(tr("Find"), this);

    // Connect actions to slots
    connect(undoAction, &QAction::triggered, this, &MenuBarEdit::_undo);
    connect(redoAction, &QAction::triggered, this, &MenuBarEdit::_redo);
    connect(cutAction, &QAction::triggered, this, &MenuBarEdit::_cut);
    connect(copyAction, &QAction::triggered, this, &MenuBarEdit::_copy);
    connect(pasteAction, &QAction::triggered, this, &MenuBarEdit::_paste);
    connect(selectAllAction, &QAction::triggered, this, &MenuBarEdit::_selectAll);
    connect(preferencesAction, &QAction::triggered, this, &MenuBarEdit::_preferences);
    connect(findAction, &QAction::triggered, this, &MenuBarEdit::_find);

    // Add keybinds
    undoAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Z), QKeySequence(Qt::CTRL | Qt::Key_U)});
    redoAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_Y), QKeySequence(Qt::CTRL | Qt::Key_R)});
    cutAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_X), QKeySequence::Cut});
    copyAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_C) , QKeySequence::Copy});
    pasteAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_V), QKeySequence::Paste});
    findAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_F), QKeySequence::Find});
    selectAllAction->setShortcuts({QKeySequence(Qt::CTRL | Qt::Key_A), QKeySequence::SelectAll});

    // Add actions to the menu
    addAction(undoAction);
    addAction(redoAction);
    addSeparator();
    addAction(cutAction);
    addAction(copyAction);
    addAction(pasteAction);
    addSeparator();
    addAction(selectAllAction);
    addSeparator();
    addAction(preferencesAction);
}

void MenuBarEdit::_undo(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit undoClicked();
}
void MenuBarEdit::_redo(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit redoClicked();
}
void MenuBarEdit::_cut(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit cutClicked();
}
void MenuBarEdit::_copy(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit copyClicked();
}
void MenuBarEdit::_paste(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit pasteClicked();
}
void MenuBarEdit::_selectAll(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit selectAllClicked();
}
void MenuBarEdit::_preferences(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit preferencesClicked();
}
void MenuBarEdit::_find(bool i_checked)
{
    Q_UNUSED(i_checked);
    emit findClicked();
}
