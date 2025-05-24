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
     void _new(bool i_checked);
     void _open(bool i_checked);
     void _save(bool i_checked);
     void _saveAs(bool i_checked);
     void _close(bool i_checked);
     void _quit(bool i_checked);

public:
    MenuBarFile(QWidget* parent = nullptr);
    ~MenuBarFile() override = default;
};


#endif //MENUBARFILECLASS_H
