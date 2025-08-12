#ifndef TABLEVIEWERTOOLBAR_H
#define TABLEVIEWERTOOLBAR_H
#include <QToolBar>
#include "Table.h"


class TableViewerToolbar : public QToolBar
{
    Q_OBJECT

signals:
    void newRow();
    void newColumn();
	void delRow();
	void delColumn();

private slots:
    void _newRow(bool i_checked);
    void _newColumn(bool i_checked);
	void _delRow(bool i_checked);
	void _delColumn(bool i_checked);

public :
    TableViewerToolbar(QWidget* parent = nullptr);
    ~TableViewerToolbar() = default;
};


#endif //TABLEVIEWERTOOLBAR_H
