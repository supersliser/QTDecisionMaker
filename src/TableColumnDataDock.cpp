//
// Created by t on 18/05/25.
//

#include "TableColumnDataDock.h"

#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <memory>

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
	m_boundsValuesLayout = new QVBoxLayout();
	m_boundsValuesButtonsLayout = new QHBoxLayout();
	m_layout->addRow(m_boundsValuesLayout);
	m_layout->addRow(m_boundsValuesButtonsLayout);
	m_addBoundValueButton = new QPushButton(this);
	m_addBoundValueButton->setText(tr("Add"));
	connect(m_addBoundValueButton, &QPushButton::pressed, this, &TableColumnDataDock::addedBoundsValue);
	m_boundsValuesButtonsLayout->addWidget(m_addBoundValueButton);
	m_removeBoundValueButton = new QPushButton(this);
	m_removeBoundValueButton->setText(tr("Remove"));
	m_boundsValuesButtonsLayout->addWidget(m_removeBoundValueButton);
	connect(m_removeBoundValueButton, &QPushButton::pressed, this, &TableColumnDataDock::removedBoundsValue);
	m_boundsValues = std::vector<std::unique_ptr<QDoubleSpinBox>>();
	connect(m_typeValue, &QComboBox::activated, this, &TableColumnDataDock::_typeChanged);
}

void TableColumnDataDock::_typeChanged(int i_type)
{
	emit typeChanged((Type)i_type);
}

void TableColumnDataDock::addedBoundsValue()
{
	m_boundsValues.push_back(std::make_unique<QDoubleSpinBox>(new QDoubleSpinBox(this)));
	m_boundsValuesLayout->addWidget(m_boundsValues.back().get());
	connect(m_boundsValues.back().get(), &QDoubleSpinBox::valueChanged, this, &TableColumnDataDock::_boundsValueChanged);
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

void TableColumnDataDock::removedBoundsValue() {
	auto index = -1;
	for (int i = 0; i < m_boundsValues.size(); i++) {
		if (m_boundsValues[i].get()->hasFocus()) {
			index = i;
		}
	}
	if (index == -1) {
		return;
	}
	for (int i = 0; i < m_boundsValues.size(); i++) {
		if (i > index) {
			m_boundsValues[i - 1] = std::make_unique<QDoubleSpinBox>(m_boundsValues[i].get());
		}
	}
}

void TableColumnDataDock::_boundsValueChanged(int i_value) {
	auto index = -1;
	for (int i = 0; i < m_boundsValues.size(); i++) {
		if (m_boundsValues[i].get() == sender())
		{
			index = i;
		}
	}
	emit(index, i_value);
}
