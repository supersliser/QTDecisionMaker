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
     void _new(bool checked);
     void _open(bool checked);
     void _save(bool checked);
     void _saveAs(bool checked);
     void _close(bool checked);
     void _quit(bool checked);

public:
    MenuBarFile(QWidget* parent = nullptr);
    ~MenuBarFile() override = default;
};


#endif //MENUBARFILECLASS_H
