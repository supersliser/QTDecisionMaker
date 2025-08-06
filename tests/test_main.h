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

// Enhanced Column Tests
    void column_constructor();
    void column_constructor_data();

    void column_setType();
    void column_setType_data();

    void column_testAutoSetType();
    void column_testAutoSetType_data();

    void column_equality();
    void column_equality_data();

    void column_indexBoundaries();
    void column_indexBoundaries_data();
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

// Enhanced Row Tests
    void row_constructor();
    void row_constructor_data();

    void row_equality();
    void row_equality_data();

    void row_indexBoundaries();
    void row_indexBoundaries_data();

    void row_totalValueBoundaries();
    void row_totalValueBoundaries_data();

    void row_nameEdgeCases();
    void row_nameEdgeCases_data();
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
// |---------------------------------------|
// DataType Tests
    void datatype_createDataType();
    void datatype_createDataType_data();

// NameType Tests  
    void nametype_initialization();
    void nametype_attemptAutoSet();
    void nametype_attemptAutoSet_data();

// BoolType Tests
    void booltype_initialization();
    void booltype_attemptAutoSet(); 
    void booltype_attemptAutoSet_data();

// RateType Tests
    void ratetype_initialization();
    void ratetype_customRange();
    void ratetype_customRange_data();
    void ratetype_attemptAutoSet();
    void ratetype_attemptAutoSet_data();

// MoneyType Tests
    void moneytype_initialization();
    void moneytype_attemptAutoSet();
    void moneytype_attemptAutoSet_data();

// NumType Tests  
    void numtype_initialization();
    void numtype_attemptAutoSet();
    void numtype_attemptAutoSet_data();

// DescType Tests
    void desctype_initialization();
    void desctype_attemptAutoSet();
    void desctype_attemptAutoSet_data();

// LinkType Tests
    void linktype_initialization();
    void linktype_attemptAutoSet();
    void linktype_attemptAutoSet_data();
};

#endif // TEST_MAIN_H
