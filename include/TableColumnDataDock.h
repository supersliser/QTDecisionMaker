//
// Created by t on 18/05/25.
//

#ifndef TABLECOLUMNDATADOCK_H
#define TABLECOLUMNDATADOCK_H
#include "TableDataDock.h"
#include <QComboBox>


class TableColumnDataDock : public TableDataDock
{
    Q_OBJECT

public:
    TableColumnDataDock(QWidget* parent = 0);
    ~TableColumnDataDock() = default;
    void setType(Type i_type);

public slots:
    void setItem(Table* i_table, int i_column);

signals:
    void typeChanged(Type i_type);

private slots:
    void _typeChanged(int i_index);

private:
    QComboBox* m_typeValue;
};


#endif //TABLECOLUMNDATADOCK_H
