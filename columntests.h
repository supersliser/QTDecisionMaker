#ifndef COLUMNTESTS_H
#define COLUMNTESTS_H

#include <QObject>
#include <QTest>

class ColumnTests : public QObject
{
    Q_OBJECT

private slots:
    void setIndex();
    void setIndex_data();

    void setDisplayIndex();
    void setDisplayIndex_data();

    void setName();
    void setName_data();

    void setImportance();
    void setImportance_data();

    void setWidth();
    void setWidth_data();
};

#endif // COLUMNTESTS_H
