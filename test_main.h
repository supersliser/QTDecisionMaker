#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <QTest>


class Tests : public QObject{
    Q_OBJECT

private slots:
// Column Tests
    void column_setIndex();
    void column_setIndex_data();

    void column_setDisplayIndex();
    void column_setDisplayIndex_data();

    void column_setName();
    void column_setName_data();

    void column_setImportance();
    void column_setImportance_data();
// |---------------------------------------|
// Row Tests
    void row_setIndex();
    void row_setIndex_data();

    void row_setDisplayIndex();
    void row_setDisplayIndex_data();

    void row_setName();
    void row_setName_data();

    void row_setTotalValue();
    void row_setTotalValue_data();
// |---------------------------------------|
// Table Tests
    void table_addHeading();
    void table_addHeading_data();
    void table_updateHeading();
    void table_updateHeading_data();
    void table_getHeading();
    void table_getHeading_data();
    void table_removeHeading();
    void table_removeHeading_data();
    void table_findHeadingIndex();
    void table_findHeadingIndex_data();

    void table_addRow();
    void table_addRow_data();
    void table_updateRow();
    void table_updateRow_data();
    void table_getRow();
    void table_getRow_data();
    void table_removeRow();
    void table_removeRow_data();
    void table_findRowIndex();
    void table_findRowIndex_data();

    void table_setTitle();
    void table_setTitle_data();

    void table_setTotalValues();
    void table_setTotalValues_data();

    void setItem();
    void setItem_data();
    void headingUpdateItemConstraints();
    void headingUpdateItemConstraints_data();
    void rowUpdateItemConstraints();
    void rowUpdateItemConstraints_data();


};

#endif // TEST_MAIN_H
