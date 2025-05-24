//
// Created by t on 18/05/25.
//

#ifndef TABLEDATADOCK_H
#define TABLEDATADOCK_H
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>

#include "Table.h"


class TableDataDock : public QDockWidget
{
    Q_OBJECT;

public:
    TableDataDock(QWidget* parent = 0);
    ~TableDataDock() = default;

public slots:
    void displayValueUpdated(std::string i_value);
    virtual void setItem(Table* i_table, int i_row, int i_column);

signals:
    void displayValueChanged(std::string i_value);
    void worthValueChanged(float i_value);

protected slots:
    void _displayValueChanged();
    void _worthValueChanged();

protected:
    QLineEdit* m_displayValue;
    QDoubleSpinBox* m_worthValue;
    QFormLayout* m_layout;
};


#endif //TABLEDATADOCK_H
