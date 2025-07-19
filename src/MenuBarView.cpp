#include "../include/MenuBarView.h"
#include <QAction>
#include <QMenu>

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

    addMenu(zoomMenu);
    addAction(zoomIncreaseAction);
    addAction(zoomDecreaseAction);
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