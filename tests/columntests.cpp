#include "test_main.h"
#include "Column.h" // Assuming this is where Column is defined

void Tests::column_setIndex() {

    QFETCH(int, input);
    QFETCH(int, expected);

    Column c = Column();
    c.setIndex(input);
    QCOMPARE(c.index(), expected);
}

void Tests::column_setIndex_data() {
    QTest::addColumn<int>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("normal") << 5 << 5;
    QTest::newRow("erroneusUpper") << 66000 << 65535;
    QTest::newRow("erroneusLower") << -2 << 0;
}

void Tests::column_setDisplayIndex() {
    QFETCH(int, input);
    QFETCH(int, expected);

    Column c = Column();
    c.setDisplayIndex(input);
    QCOMPARE(c.displayIndex(), expected);
}

void Tests::column_setDisplayIndex_data() {
    QTest::addColumn<int>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("normal") << 5 << 5;
    QTest::newRow("erroneusUpper") << 66000 << 65535;
    QTest::newRow("erroneusLower") << -2 << 0;
}

void Tests::column_setName() {
    QFETCH(QString, input);
    QFETCH(QString, expected);

    Column c = Column();
    c.setName(input.toStdString());
    QCOMPARE(c.name().data(), expected.toStdString());
}

void Tests::column_setName_data() {
    QTest::addColumn<QString>("input");
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
    QTest::newRow("EmptyName") << "" << "name";
}

void Tests::column_setImportance() {
    QFETCH(int, input);
    QFETCH(int, expected);

    Column c = Column();
    c.setImportance(input);
    QCOMPARE(c.importance(), expected);
}

void Tests::column_setImportance_data() {
    QTest::addColumn<int>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("normal") << 20 << 20;
    QTest::newRow("erroneusHigh") << 300 << 127;
    QTest::newRow("erroneusLow") << -300 << -128;
}
