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
    setWindowTitle("Column Details");

    m_typeValue = new QComboBox(this);
    m_layout->addRow(m_typeValue);
    m_typeValue->setEditable(true);
    for (int i = 0; i < 7; i++)
    {
        m_typeValue->addItem(tr(DataType::createDataType((Type)i)->name().data()));
    }

    connect(m_typeValue, &QComboBox::activated, this, &TableColumnDataDock::_typeChanged);
}

void TableColumnDataDock::_typeChanged(int i_type)
{
    emit typeChanged((Type)i_type);
}

void TableColumnDataDock::setItem(Table* i_table, int i_column)
{
    int nameColumn = 0;
    int totalValueColumn = i_table->headingCount() + 1;

    if (nameColumn == i_column)
    {
        m_displayValue->setText("Name");
        m_displayValue->setReadOnly(true);
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
        m_typeValue->setCurrentIndex(Type::NAME);
        m_typeValue->setEditable(false);
    }
    else if (totalValueColumn == i_column)
    {
        m_displayValue->setText("Total Value");
        m_displayValue->setReadOnly(true);
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
        m_typeValue->setCurrentIndex(Type::NUM);
        m_typeValue->setEditable(false);
    }
    else if (i_column > 0)
    {
        m_displayValue->setText(i_table->heading(i_column - 1)->name().c_str());
        m_displayValue->setReadOnly(false);
        m_worthValue->setValue(i_table->heading(i_column - 1)->importance());
        m_worthValue->setReadOnly(false);
        m_typeValue->setCurrentIndex(i_table->heading(i_column - 1)->type().type());
        m_typeValue->setEditable(true);
    }
}

void TableColumnDataDock::setType(Type i_type)
{
    m_typeValue->setCurrentIndex(i_type);
}
