//
// Created by t on 24/06/25.
//

#ifndef MENUBAREDIT_H
#define MENUBAREDIT_H
#include <QMenu>

class MenuBarEdit : public QMenu
{
    Q_OBJECT
    signals:
    void undoClicked();
    void redoClicked();
    void cutClicked();
    void copyClicked();
    void pasteClicked();
    void selectAllClicked();
    void preferencesClicked();
    void findClicked();

    private slots:
    void _undo(bool i_checked);
    void _redo(bool i_checked);
    void _cut(bool i_checked);
    void _copy(bool i_checked);
    void _paste(bool i_checked);
    void _selectAll(bool i_checked);
    void _preferences(bool i_checked);
    void _find(bool i_checked);

public:
    MenuBarEdit(QWidget* parent = nullptr);
    ~MenuBarEdit() override = default;
};
#endif //MENUBAREDIT_H
