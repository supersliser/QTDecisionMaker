//
// Created by t on 17/05/25.
//

#include "../include/tableManager.h"

#include <fmt/format.h>
#include <QHeaderView>

tableManager::tableManager(QWidget* parent)
    : QTableWidget(parent)
{
    connect(this, &QTableWidget::currentCellChanged, this, &tableManager::_selectItem);
}

void tableManager::_selectItem(int row, int column, int prev_row, int prev_column)
{
    if (row != -1 && column != -1)
    {
        _lastSelectedRow = row;
        _lastSelectedColumn = column;
    }
    emit selectItem(row, column);
}

void tableManager::drawTable(Table* data)
{
    clear();
    setColumnCount(data->headingCount() + 2);
    _setColumnHeader(0, tr("Name"));
    for (int i = 1; i < data->headingCount() + 1; i++)
    {
        _setColumnHeader(i, tr(data->heading(i - 1)->name().data()));
    }
    _setColumnHeader(data->headingCount() + 1, tr("Total Value"));

    data->calculateAllTotals();

    setRowCount(data->rowCount());
    for (int r = 0; r < data->rowCount(); r++)
    {
        _setItem(r, 0, tr(data->row(r)->name().data()));
        for (int c = 1; c < data->headingCount() + 1; c++)
        {
            _setItem(r, c, tr(data->item(c - 1, r)->displayValue.data()));
        }
        _setItem(r, data->headingCount() + 1, fmt::format("{:.2f}", data->row(r)->totalValue()).c_str());
    }

    show();
}

void tableManager::_setColumnHeader(int i_column, QString i_name)
{
    setHorizontalHeaderItem(i_column, new QTableWidgetItem(i_name));
}

void tableManager::_setItem(int i_row, int i_column, QString i_name)
{
    setItem(i_row, i_column, new QTableWidgetItem(i_name));
}

void tableManager::_itemEdited(int row, int column)
{
    emit itemEdited(currentItem()->text().toStdString());
}

int tableManager::selectedColumn()
{
    return _lastSelectedColumn;
}

int tableManager::selectedRow()
{
    return _lastSelectedRow;
}

