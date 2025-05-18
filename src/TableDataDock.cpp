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

void TableDataDock::displayValueUpdated(std::string value)
{
    m_displayValue->setText(QString(value.c_str()));
}

void TableDataDock::setItem(Table* table, int row, int column)
{
    int nameColumn = 0;
    int totalValueColumn = table->headingCount() + 1;

    if (nameColumn == column)
    {
        m_displayValue->setText(table->row(row)->name().c_str());
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
    }
    else if (totalValueColumn == column)
    {
        m_displayValue->setText(std::to_string(table->row(row)->totalValue()).c_str());
        m_worthValue->setValue(0);
        m_worthValue->setReadOnly(true);
    }
    else if (row >= 0 && column > 0)
    {
        m_displayValue->setText(table->item(column - 1, row)->displayValue.c_str());
        m_worthValue->setReadOnly(false);
        m_worthValue->setValue(table->item(column - 1, row)->worthValue);
    }
}
