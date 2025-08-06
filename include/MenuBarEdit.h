//
// Created by t on 24/06/25.
//

#ifndef MENUBAREDIT_H
#define MENUBAREDIT_H
#include <QMenu>

class MenuBarEdit : public QMenu
{
    Q_OBJECT

public:
    MenuBarEdit(QWidget* parent = nullptr);
    ~MenuBarEdit() override = default;

signals:
    void reorderColumns();
    void reorderRows();

private slots:
    void _showReorderColumnsDialog();
    void _showReorderRowsDialog();
};
#endif //MENUBAREDIT_H
