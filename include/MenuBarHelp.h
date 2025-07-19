//
// Created by copilot for issue #24
//

#ifndef MENUBARHELP_H
#define MENUBARHELP_H
#include <QMenu>

class MenuBarHelp : public QMenu
{
    Q_OBJECT

public:
    MenuBarHelp(QWidget* parent = nullptr);
    ~MenuBarHelp() override = default;
};

#endif //MENUBARHELP_H