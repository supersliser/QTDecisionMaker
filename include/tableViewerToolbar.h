#ifndef TABLEVIEWERTOOLBAR_H
#define TABLEVIEWERTOOLBAR_H
#include <QToolBar>
#include "table.h"


class tableViewerToolbar : public QToolBar
{
    Q_OBJECT

signals:
    void newRow();
    void newColumn();

private slots:
    void _newRow(bool checked);
    void _newColumn(bool checked);

public :
    tableViewerToolbar(QWidget* parent = nullptr);
    ~tableViewerToolbar() = default;
};


#endif //TABLEVIEWERTOOLBAR_H
