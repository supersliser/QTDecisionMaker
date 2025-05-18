//
// Created by t on 17/05/25.
//

#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>

#include "column.h"
#include "row.h"
#include "table.h"


class tableManager : public QTableWidget
{
    Q_OBJECT

public:
    tableManager(QWidget* parent = nullptr);
    ~tableManager() = default;

signals:
    void selectItem(int row, int column);
    void itemEdited(std::string value);

private slots:
    void _selectItem(int row, int column, int prev_row, int prev_column);
    void _itemEdited(int row, int column);

public slots:
    void drawTable(Table* data);

private:
    void _setColumnHeader(int i_column, QString i_name);
    void _setItem(int i_row, int i_column, QString i_name);
};


#endif //TABLEMANAGER_H
