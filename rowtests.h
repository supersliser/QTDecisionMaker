#ifndef ROWTESTS_H
#define ROWTESTS_H

#include <QObject>
#include <QTest>

class RowTests : public QObject
{
    Q_OBJECT

private slots:
    void setIndex();
    void setIndex_data();

    void setDisplayIndex();
    void setDisplayIndex_data();

    void setName();
    void setName_data();

    void setTotalValue();
    void setTotalValue_data();
};

#endif // ROWTESTS_H
