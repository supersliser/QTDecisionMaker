//
// Created by t on 14/05/25.
//

#ifndef MENUBARFILECLASS_H
#define MENUBARFILECLASS_H
#include <QMenu>


class MenuBarFile : public QMenu
{
    Q_OBJECT

public:
    MenuBarFile(QWidget* parent = nullptr);
    ~MenuBarFile() override = default;
};


#endif //MENUBARFILECLASS_H
