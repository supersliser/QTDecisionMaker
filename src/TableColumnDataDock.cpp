//
// Created by t on 18/05/25.
//

#include "TableColumnDataDock.h"

#include <QComboBox>

TableColumnDataDock::TableColumnDataDock(QWidget* parent)
    : TableDataDock(parent)
{
    setMaximumSize(QSize(200, 300));
    setMinimumSize(QSize(200, 300));
    setWindowTitle("Item Details");

    m_typeValue = new QComboBox(this);
    m_layout->addRow(m_typeValue);
    m_typeValue->setEditable(true);
    for (int i = 0; i < 7; i++)
    {
        m_typeValue->addItem(tr(DataType::createDataType((Type)i)->getName().data()));
    }

    connect(m_typeValue, &QComboBox::activated, this, &TableColumnDataDock::_typeChanged);
}

void TableColumnDataDock::_typeChanged(int type)
{
    emit typeChanged((Type)type);
}

void TableColumnDataDock::setItem(Table* table, int column)
{
    int nameColumn = 0;
    int totalValueColumn = table->headingCount() + 1;

    if (nameColumn == column)
    {
        m_displayValue->setText("Name");
        m_displayValue->setReadOnly(true);
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
        m_typeValue->setCurrentIndex(Type::NAME);
        m_typeValue->setEditable(false);
    }
    else if (totalValueColumn == column)
    {
        m_displayValue->setText("Total Value");
        m_displayValue->setReadOnly(true);
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
        m_typeValue->setCurrentIndex(Type::NUM);
        m_typeValue->setEditable(false);
    }
    else if (column > 0)
    {
        m_displayValue->setText(table->heading(column - 1)->name().c_str());
        m_displayValue->setReadOnly(false);
        m_worthValue->setValue(table->heading(column - 1)->importance());
        m_worthValue->setReadOnly(false);
        m_typeValue->setCurrentIndex(table->heading(column - 1)->type().getType());
        m_typeValue->setEditable(true);
    }
}

void TableColumnDataDock::setType(Type type)
{
    m_typeValue->setCurrentIndex(type);
}
