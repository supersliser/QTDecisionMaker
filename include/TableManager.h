//
// Created by t on 17/05/25.
//

#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>
#include <stack>

#include "Column.h"
#include "Row.h"
#include "Table.h"


class TableManager : public QTableWidget
{
    Q_OBJECT

public:
    TableManager(QWidget* parent = nullptr);
    ~TableManager() = default;
    int selectedColumn();
    int selectedRow();

signals:
    void selectItem(int i_row, int i_column);
    void itemEdited(std::string i_value);

private slots:
    void _selectItem(int i_row, int i_column, int i_prev_row, int i_prev_column);
    void _itemEdited(int i_row, int i_column);

public slots:
    void drawTable(Table* i_data);
    void findTriggered(const QString& i_text);

private:
    void _setColumnHeader(int i_column, QString i_name);
    void _setItem(int i_row, int i_column, Item *i_item);
    void _setItemName(int i_row, int i_column, QString i_name);
    int _m_lastSelectedRow = -1;
    int _m_lastSelectedColumn = -1;
};


#endif //TABLEMANAGER_H
