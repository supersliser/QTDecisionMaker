//
// Created by t on 14/05/25.
//

#ifndef MENUBARFILECLASS_H
#define MENUBARFILECLASS_H
#include <QMenu>


class MenuBarFile : public QMenu
{
    Q_OBJECT

signals:
    void newClicked();
    void openClicked();
    void saveClicked();
    void saveAsClicked();
    void closeClicked();
    void quitClicked();

private slots:
    // void _new();
    // void _open();
    // void _save();
    // void _saveAs();
    // void _close();
    // void _quit();

public:
    MenuBarFile(QWidget* parent = nullptr);
    ~MenuBarFile() override = default;
};


#endif //MENUBARFILECLASS_H
