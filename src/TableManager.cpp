//
// Created by t on 17/05/25.
//

#include "../include/TableManager.h"

#include <QApplication>
#include <fmt/format.h>
#include <QHeaderView>

TableManager::TableManager(QWidget* parent)
    : QTableWidget(parent)
{
    _M_ORIGINAL_SIZE = QApplication::font().pointSize();
    connect(this, &QTableWidget::currentCellChanged, this, &TableManager::_selectItem);
}

void TableManager::_selectItem(int i_row, int i_column, int i_prev_row, int i_prev_column)
{
    Q_UNUSED(i_prev_column);
    Q_UNUSED(i_prev_row);
    if (i_row != -1 && i_column != -1)
    {
        _m_lastSelectedRow = i_row;
        _m_lastSelectedColumn = i_column;
    }
    emit selectItem(i_row, i_column);
}

void TableManager::drawTable(Table* i_data)
{
    clear();
    setColumnCount(i_data->headingCount() + 2);
    _setColumnHeader(0, tr("Name"));
    for (int i = 1; i < i_data->headingCount() + 1; i++)
    {
        _setColumnHeader(i, tr(i_data->heading(i - 1)->name().data()));
    }
    _setColumnHeader(i_data->headingCount() + 1, tr("Total Value"));

    i_data->calculateAllTotals();

    setRowCount(i_data->rowCount());
    for (int r = 0; r < i_data->rowCount(); r++)
    {
        _setItemName(r, 0, tr(i_data->row(r)->name().data()));
        for (int c = 1; c < i_data->headingCount() + 1; c++)
        {
            _setItem(r, c, i_data->item(c - 1, r));
        }
        _setItemName(r, i_data->headingCount() + 1, fmt::format("{:.2f}", i_data->row(r)->totalValue()).c_str());
    }

    show();
}

void TableManager::_setColumnHeader(int i_column, QString i_name)
{
    setHorizontalHeaderItem(i_column, new QTableWidgetItem(i_name));
}

void TableManager::_setItemName(int i_row, int i_column, QString i_name)
{
    setItem(i_row, i_column, new QTableWidgetItem(i_name));
}

void TableManager::_setItem(int i_row, int i_column, Item* i_item)
{
    auto item = new QTableWidgetItem(i_item->displayValue.data());
    QColor color;

    if (i_item->worthValue > 0) {
        color = QColor(0, 255, 0); // Green for worth value >= 1
    } else if (i_item->worthValue < 0) {
        color = QColor(255, 0, 0); // Red for worth value < 0
    } else {
        color = QColor(255, 255, 0); // Yellow for worth value == 0
    }

    item->setBackground(color);
    setItem(i_row, i_column, item);
}

void TableManager::_itemEdited(int i_row, int i_column)
{
    Q_UNUSED(i_row);
    Q_UNUSED(i_column);
    emit itemEdited(currentItem()->text().toStdString());
}

int TableManager::selectedColumn()
{
    return _m_lastSelectedColumn;
}

int TableManager::selectedRow()
{
    return _m_lastSelectedRow;
}

void TableManager::findTriggered(const QString& i_text)
{
    for (int r = 0; r < rowCount(); r++)
    {
        for (int c = 0; c < columnCount(); c++)
        {
            if (item(r, c) && item(r, c)->text().contains(i_text, Qt::CaseInsensitive))
            {
                setCurrentCell(r, c);
                return;
            }
        }
    }
}

void TableManager::zoomChanged(float i_newZoom)
{
    auto f = QApplication::font();
    f.setPointSize(_M_ORIGINAL_SIZE * (i_newZoom / 100.0f));
    QApplication::setFont(f);
}