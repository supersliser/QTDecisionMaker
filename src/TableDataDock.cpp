//
// Created by t on 18/05/25.
//

#include "../include/TableDataDock.h"

TableDataDock::TableDataDock(QWidget* parent)
    : QDockWidget(parent)
{
    setMinimumSize(QSize(200, 200));
    setMaximumSize(QSize(200, 200));
    setWindowTitle("Item Details");

    auto temp = new QWidget(this);
    setWidget(temp);
    m_layout = new QFormLayout(temp);
    m_layout->setContentsMargins(10, 10, 10, 10);
    m_layout->setSpacing(10);

    m_displayValue = new QLineEdit(this);
    m_layout->addRow(m_displayValue);

    m_worthValue = new QDoubleSpinBox(this);
    m_layout->addRow(m_worthValue);
    m_worthValue->setDecimals(2);
    m_worthValue->setMinimum(-100);
    m_worthValue->setMaximum(100);
    m_worthValue->setSingleStep(1);
    m_worthValue->setButtonSymbols(QAbstractSpinBox::PlusMinus);

    connect(m_displayValue, &QLineEdit::editingFinished, this, &TableDataDock::_displayValueChanged);
    connect(m_worthValue, &QDoubleSpinBox::editingFinished, this, &TableDataDock::_worthValueChanged);
}

void TableDataDock::_displayValueChanged()
{
    emit displayValueChanged(m_displayValue->text().toStdString());
}

void TableDataDock::_worthValueChanged()
{
    emit worthValueChanged(m_worthValue->value());
}

void TableDataDock::displayValueUpdated(std::string i_value)
{
    m_displayValue->setText(QString(i_value.c_str()));
}

void TableDataDock::setItem(Table* i_table, int i_row, int i_column)
{
    int nameColumn = 0;
    int totalValueColumn = i_table->headingCount() + 1;

    if (nameColumn == i_column)
    {
        m_displayValue->setText(i_table->row(i_row)->name().c_str());
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
    }
    else if (totalValueColumn == i_column)
    {
        m_displayValue->setText(std::to_string(i_table->row(i_row)->totalValue()).c_str());
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
    }
    else if (i_row >= 0 && i_column > 0)
    {
        m_displayValue->setText(i_table->item(i_column - 1, i_row)->displayValue.c_str());
        m_worthValue->setReadOnly(false);
        m_worthValue->setValue(i_table->item(i_column - 1, i_row)->worthValue);
    }
}
