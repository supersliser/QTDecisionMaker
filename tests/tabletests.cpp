#include "test_main.h"
#include "Column.h"
#include "Table.h"
#include "Row.h"
#include <array>
#include <iostream>

void Tests::table_addHeading() {
    QFETCH(std::vector<Column>, input);
    QFETCH(int, columnCount);
    QFETCH(std::vector<Column>, expected);

    Table t = Table();
    for (int i = 0; i < columnCount; i++) {
        t.addHeading(input[i]);
    }
    for (int i = 0; i < columnCount; i++) {
        Column* col = t.heading(i);
        QCOMPARE(t.heading(i)->name(), expected[i].name());
        QCOMPARE(t.heading(i)->importance(), expected[i].importance());
    }
    for (int i = 0; i < t.headingCount(); i++) {
        QCOMPARE(t.heading(i)->index(), i);
        QCOMPARE(t.heading(i)->displayIndex(), i);
    }
}

void Tests::table_addHeading_data() {
    QTest::addColumn<std::vector<Column>>("input");
    QTest::addColumn<int>("columnCount");
    QTest::addColumn<std::vector<Column>>("expected");

    std::vector<Column> defaultColumnList = {Column()};
    QTest::newRow("defaultColumn") << defaultColumnList << 1 << defaultColumnList;
    std::vector<Column> defaultColumnTwiceList = {Column(), Column()};
    QTest::newRow("defaultColumnTwice") << defaultColumnTwiceList << 2 << defaultColumnTwiceList;

    std::vector<Column> customNameList = {Column("TEST_NAME")};
    QTest::newRow("customNameColumn") << customNameList << 1 << customNameList;
    std::vector<Column> customNameColumnTwiceList = {Column("TEST_NAME1"), Column("TEST_NAME2")};
    QTest::newRow("customNameColumnTwice") << customNameColumnTwiceList << 2 << customNameColumnTwiceList;

    std::vector<Column> thousandList;
    for (int i = 0; i < 1000; i++) {
        thousandList.emplace_back(Column());
    }
    QTest::newRow("thousandColumns") << thousandList << 1000 << thousandList;
}

void Tests::table_updateHeading() {
    QFETCH(QString, inputName);
    QFETCH(int, inputWorth);
    QFETCH(QString, expectedName);
    QFETCH(int, expectedWorth);

    Table t;
    t.addHeading(Column());
    Table t2 = t;
    t.heading(0)->setName(inputName.toStdString());
    QCOMPARE(t.heading(0)->name(), expectedName.toStdString());
    t.heading(0)->setImportance(inputWorth);
    QCOMPARE(t.heading(0)->importance(), expectedWorth);

    Column* c = t2.heading(0);

    QCOMPARE_NE(c->name(), expectedName.toStdString());

    c->setName(inputName.toStdString());
    c->setImportance(inputWorth);
    QCOMPARE(c->name(), expectedName.toStdString());
    QCOMPARE(c->importance(), expectedWorth);
}

void Tests::table_updateHeading_data() {
    QTest::addColumn<QString>("inputName");
    QTest::addColumn<QString>("expectedName");
    QTest::addColumn<int>("inputWorth");
    QTest::addColumn<int>("expectedWorth");

    QTest::newRow("normal") << "TEST_NAME" << "TEST_NAME" << 5 << 5;
    QTest::newRow("fixName") << "TEST  NAME" << "TEST NAME" << 5 << 5;
    QTest::newRow("fixImportance") << "Name" << "Name" << -300 << -128;
}

void Tests::table_getHeading() {
    QFETCH(Column, inputColumn);
    QFETCH(Column, expectedColumn);

    Table t;
    t.addHeading(inputColumn);
    QCOMPARE(t.heading(0)->name(), "TEST");
}

void Tests::table_getHeading_data() {
    QTest::addColumn<Column>("inputColumn");
    QTest::addColumn<Column>("expectedColumn");

        QTest::newRow("normal") << Column("TEST") << Column("TEST");
}

void Tests::table_removeHeading() {
    QFETCH(std::vector<Column>, columnsToAdd);
    QFETCH(std::vector<Column>, expectedColumnsAfterwards);
    QFETCH(int, indexToRemove);

    Table t;
    for (auto c : columnsToAdd) {
        t.addHeading(c);
    }
    QCOMPARE(t.headingCount(), columnsToAdd.size());
    t.removeHeading(indexToRemove);
    QCOMPARE(t.headingCount(), expectedColumnsAfterwards.size());
    for (int i = 0; i < expectedColumnsAfterwards.size(); i ++) {
        QCOMPARE(t.heading(i)->name(), expectedColumnsAfterwards[i].name());
    }
}

void Tests::table_removeHeading_data() {
    QTest::addColumn<std::vector<Column>>("columnsToAdd");
    QTest::addColumn<std::vector<Column>>("expectedColumnsAfterwards");
    QTest::addColumn<int>("indexToRemove");

    std::vector<Column> addColumns = {Column(), Column("TEST1"), Column("TEST2")};
    std::vector<Column> expectColumns = {Column(), Column("TEST2")};
    QTest::newRow("normal") << addColumns << expectColumns << 1;
    QTest::newRow("indexOutside") << addColumns << addColumns << 27;
}

void Tests::table_findHeadingIndex() {
    QFETCH(std::vector<Column>, columns);
    QFETCH(std::function<bool(Column i_value)>, predicate);
    QFETCH(int, expected);

    Table t;
    for (auto c : columns) {
        t.addHeading(c);
    }
    int output = t.findHeading(predicate);
    QCOMPARE(output, expected);
}

void Tests::table_findHeadingIndex_data() {
    QTest::addColumn<std::vector<Column>>("columns");
    QTest::addColumn<std::function<bool(Column i_value)>>("predicate");
    QTest::addColumn<int>("expected");

    std::vector<Column> normalColumns = {Column("itemOne", false), Column("itemTwo", 0, false), Column("itemThree", 0, false), Column("itemFour", 0, false), Column("itemFive", 0, false)};
    std::function<bool(Column i_value)> p1 = [](Column i_value) {
        return i_value.name() == "itemFour";
    };
    std::function<bool(Column i_value)> p2 = [](Column i_value) {
        return i_value.index() == 3;
    };
    std::function<bool(Column i_value)> p3 = [](Column i_value) {
        return i_value.name() == "itemSixteen";
    };
    std::function<bool(Column i_value)> p4 = [](Column i_value) {
        return i_value.index() == 20;
    };
    QTest::newRow("normalWithEqual") << normalColumns << p1 << 3;
    QTest::newRow("normalWithIndex") << normalColumns << p2 << 3;
    QTest::newRow("erroneusPredicateWithEqual") << normalColumns << p3 << -1;
    QTest::newRow("erroenusPredicateWithIndex") << normalColumns << p4 << -1;
}

void Tests::table_addRow() {
    QFETCH(std::vector<Row>, input);
    QFETCH(int, rowCount);
    QFETCH(std::vector<Row>, expected);

    Table t = Table();
    for (int i = 0; i < rowCount; i++) {
        t.addRow(input[i]);
    }
    for (int i = 0; i < rowCount; i++) {
        QCOMPARE(t.row(i+1)->name(), expected[i].name());
    }
    for (int i = 0; i < t.rowCount(); i++) {
        QCOMPARE(t.row(i)->index(), i);
        QCOMPARE(t.row(i)->displayIndex(), i);
    }
}

void Tests::table_addRow_data() {
    QTest::addColumn<std::vector<Row>>("input");
    QTest::addColumn<int>("rowCount");
    QTest::addColumn<std::vector<Row>>("expected");

    std::vector<Row> defaultRowList = {Row()};
    QTest::newRow("defaultRow") << defaultRowList << 1 << defaultRowList;
    std::vector<Row> defaultRowTwiceList = {Row(), Row()};
    QTest::newRow("defaultRowTwice") << defaultRowTwiceList << 2 << defaultRowTwiceList;

    std::vector<Row> customNameList = {Row("TEST_NAME")};
    QTest::newRow("customNameRow") << customNameList << 1 << customNameList;
    std::vector<Row> customNameRowTwiceList = {Row("TEST_NAME1"), Row("TEST_NAME2")};
    QTest::newRow("customNameRowTwice") << customNameRowTwiceList << 2 << customNameRowTwiceList;

    std::vector<Row> thousandList;
    for (int i = 0; i < 1000; i++) {
        thousandList.emplace_back(Row());
    }
    QTest::newRow("thousandRows") << thousandList << 1000 << thousandList;
}

void Tests::table_updateRow() {
    QFETCH(QString, inputName);
    QFETCH(QString, expectedName);

    Table t;
    t.addRow(Row());
    t.row(1);
    Table t2 = t;
    t.row(1)->setName(inputName.toStdString());
    QCOMPARE(t.row(1)->name(), expectedName.toStdString());

    Row* r = t2.row(1);

    QCOMPARE_NE(r->name(), expectedName.toStdString());

    r->setName(inputName.toStdString());
    QCOMPARE(r->name(), expectedName.toStdString());
}

void Tests::table_updateRow_data() {
    QTest::addColumn<QString>("inputName");
    QTest::addColumn<QString>("expectedName");

    QTest::newRow("normal") << "TEST_NAME" << "TEST_NAME";
    QTest::newRow("fixName") << "TEST  NAME" << "TEST NAME";
}

void Tests::table_getRow() {
    QFETCH(Row, inputRow);
    QFETCH(Row, expectedRow);

    Table t;
    t.addRow(inputRow);
    QCOMPARE(t.row(0)->name(), "Blank Name");
    QCOMPARE(t.row(3), nullptr);
    QCOMPARE(t.row(1)->name(), expectedRow.name());
}

void Tests::table_getRow_data() {
    QTest::addColumn<Row>("inputRow");
    QTest::addColumn<Row>("expectedRow");

        QTest::newRow("normal") << Row("TEST") << Row("TEST");
}

void Tests::table_removeRow() {
    QFETCH(std::vector<Row>, rowsToAdd);
    QFETCH(int, indexToRemove);
    QFETCH(std::vector<Row>, expectedRowsAfterwards);

    Table t;
    for (auto r : rowsToAdd) {
        t.addRow(r);
    }
    QCOMPARE(t.rowCount(), rowsToAdd.size() + 1);
    t.removeRow(indexToRemove);
    QCOMPARE(t.rowCount(), expectedRowsAfterwards.size() + 1);
    for (int i = 0; i < expectedRowsAfterwards.size(); i ++) {
        QCOMPARE(*t.row(i + 1), expectedRowsAfterwards[i]);
    }
}

void Tests::table_removeRow_data() {
    QTest::addColumn<std::vector<Row>>("rowsToAdd");
    QTest::addColumn<std::vector<Row>>("expectedRowsAfterwards");
    QTest::addColumn<int>("indexToRemove");

    std::vector<Row> addRows = {Row(), Row("TEST1"), Row("TEST2")};
    std::vector<Row> expectRows = {Row(), Row("TEST2")};
    QTest::newRow("normal") << addRows << expectRows << 2;
    QTest::newRow("indexOutside") << addRows << addRows << 27;
}

void Tests::table_findRowIndex() {
    QFETCH(std::vector<Row>, rows);
    QFETCH(std::function<bool(Row i_value)>, predicate);
    QFETCH(int, expected);

    Table t;
    for (auto r : rows) {
        t.addRow(r);
    }
    int output = t.findRow(predicate);
    QCOMPARE(output, expected);
}

void Tests::table_findRowIndex_data() {
    QTest::addColumn<std::vector<Row>>("rows");
    QTest::addColumn<std::function<bool(Row i_value)>>("predicate");
    QTest::addColumn<int>("expected");

    std::vector<Row> normalRows = {Row("itemOne"), Row("itemTwo"), Row("itemThree"), Row("itemFour"), Row("itemFive")};
    std::function<bool(Row i_value)> p1 = [](Row i_value) {
        return i_value.name() == "itemFour";
    };
    std::function<bool(Row i_value)> p2 = [](Row i_value) {
        return i_value.index() == 3;
    };
    std::function<bool(Row i_value)> p3 = [](Row i_value) {
        return i_value.name() == "itemSixteen";
    };
    std::function<bool(Row i_value)> p4 = [](Row i_value) {
        return i_value.index() == 20;
    };
    QTest::newRow("normalWithEqual") << normalRows << p1 << 4;
    QTest::newRow("normalWithIndex") << normalRows << p2 << 3;
    QTest::newRow("erroneusPredicateWithEqual") << normalRows << p3 << -1;
    QTest::newRow("erroenusPredicateWithIndex") << normalRows << p4 << -1;
}

void Tests::table_setTitle() {
    QFETCH(QString, title);
    QFETCH(QString, expected);

    Table t;
    t.setTitle(title.toStdString());
    QCOMPARE(t.title(), expected.toStdString());
}

void Tests::table_setTitle_data() {
    QTest::addColumn<QString>("title");
    QTest::addColumn<QString>("expected");

    QTest::newRow("correct") << "Test_name" << "Test_name";
    QTest::newRow("spaced") << "Test Name" << "Test Name";
    QTest::newRow("newline") << "Test\nName" << "Test Name";
    QTest::newRow("doubleNewLine") << "Test\n\nName" << "Test Name";
    QTest::newRow("newLineAtStart") << "\nTest Name" << "Test Name";
    QTest::newRow("NewLineAtEnd") << "Test Name\n" << "Test Name";
    QTest::newRow("AllnewLines") << "\nTest\n\nName\n" << "Test Name";
    QTest::newRow("DoubleSpace") << "Test  Name" << "Test Name";
    QTest::newRow("DoubleSpaceWithNewLine") << "Test \nName" << "Test Name";
    QTest::newRow("SpaceAtFront") << " Test Name" << "Test Name";
    QTest::newRow("SpaceAtEnd") << "Test Name " << "Test Name";
    QTest::newRow("EmptyName") << "" << "Title";

}

void Tests::table_setTotalValues() {
    QFETCH(Table, table);
    QFETCH(std::vector<float>, expectedTotals);

    table.calculateAllTotals();
    for (int i = 0; i < expectedTotals.size(); i++) {
        QCOMPARE(table.row(i)->totalValue(), expectedTotals[i]);
    }
}

void Tests::table_setTotalValues_data() {
    QTest::addColumn<Table>("table");
    QTest::addColumn<std::vector<float>>("expectedTotals");

    Table t = Table();
    t.addHeading(Column("Column 1"));
    t.addHeading(Column("Column 2"));
    t.addRow(Row("Row 1"));
    t.addRow(Row("Row 2"));
    t.heading(0)->setImportance(5);
    t.heading(1)->setImportance(-3);
    t.item(0, 0)->worthValue = 1;
    t.item(0, 1)->worthValue = 0;
    t.item(1, 0)->worthValue = -1;
    t.item(1, 1)->worthValue = -1;
    std::vector<float> expected = {8, 3};

    QTest::newRow("normal") << t << expected;
    t.item(0, 0)->worthValue = 0;
    t.item(0, 1)->worthValue = 0;
    t.item(1, 0)->worthValue = 0;
    t.item(1, 1)->worthValue = 0;
    expected = {0, 0};
    QTest::newRow("zero") << t << expected;
}

