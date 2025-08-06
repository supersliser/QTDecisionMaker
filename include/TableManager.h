//
// Created by t on 17/05/25.
//

#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include <QTableWidget>
#include <stack>
#include <QHeaderView>

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
    void zoomChanged(float i_newZoom);

signals:
    void selectItem(int i_row, int i_column);
    void itemEdited(std::string i_value);
    void columnReordered(int fromIndex, int toIndex);
    void rowReordered(int fromIndex, int toIndex);

private slots:
    void _selectItem(int i_row, int i_column, int i_prev_row, int i_prev_column);
    void _itemEdited(int i_row, int i_column);
    void _onColumnMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void _onRowMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

public slots:
    void drawTable(Table* i_data);
    void findTriggered(const QString& i_text);
    void filterTable(const QString& filterText);

private:
    void _setColumnHeader(int i_column, QString i_name);
    void _setItem(int i_row, int i_column, Item *i_item);
    void _setItemName(int i_row, int i_column, QString i_name);
    int _m_lastSelectedRow = -1;
    int _m_lastSelectedColumn = -1;

    float _M_ORIGINAL_SIZE;
};


#endif //TABLEMANAGER_H
