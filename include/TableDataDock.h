//
// Created by t on 18/05/25.
//

#ifndef TABLEDATADOCK_H
#define TABLEDATADOCK_H
#include <QDockWidget>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QLineEdit>

#include "table.h"


class TableDataDock : public QDockWidget
{
    Q_OBJECT;

public:
    TableDataDock(QWidget* parent = 0);
    ~TableDataDock() = default;

public slots:
    void displayValueUpdated(std::string value);
    virtual void setItem(Table* table, int row, int column);

signals:
    void displayValueChanged(std::string value);
    void worthValueChanged(float value);

protected slots:
    void _displayValueChanged();
    void _worthValueChanged();

protected:
    QLineEdit* m_displayValue;
    QDoubleSpinBox* m_worthValue;
    QFormLayout* m_layout;
};


#endif //TABLEDATADOCK_H
