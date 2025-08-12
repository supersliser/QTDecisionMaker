//
// Created by t on 18/05/25.
//

#ifndef TABLECOLUMNDATADOCK_H
#define TABLECOLUMNDATADOCK_H
#include "TableDataDock.h"
#include <QComboBox>
#include <vector>
#include <QPushButton>


class TableColumnDataDock : public TableDataDock
{
    Q_OBJECT

public:
    TableColumnDataDock(QWidget* parent = 0);
    ~TableColumnDataDock() = default;
    void setType(Type i_type);

public slots:
	void setItem(Table* i_table, int i_column);
	void addedBoundsValue();
	void removedBoundsValue();

signals:
	void typeChanged(Type i_type);
	void boundsValueChanged(int i_index, int i_value);
	void boundsValueAdded();
	void boundsValueRemoved(int i_index);

private slots:
	void _typeChanged(int i_index);
	void _boundsValueChanged(int i_value);
	void _boundsValueSelected();
private:
	int m_lastBoundsValueSelected;
	QComboBox* m_typeValue;
	QVBoxLayout* m_boundsValuesLayout;
	QHBoxLayout* m_boundsValuesButtonsLayout;
	QPushButton* m_addBoundValueButton;
	QPushButton* m_removeBoundValueButton;
	std::vector<std::unique_ptr<QDoubleSpinBox>> m_boundsValues;
	void addBoundValueRun();
};


#endif //TABLECOLUMNDATADOCK_H
