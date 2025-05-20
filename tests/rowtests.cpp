#include "test_main.h"
#include "Row.h"
#include <cstdlib>
#include <cstdint>

void Tests::row_setIndex() {

    QFETCH(int, input);
    QFETCH(int, expected);

    Row c = Row();
    c.setIndex(input);
    QCOMPARE(c.index(), expected);
}

void Tests::row_setIndex_data() {
    QTest::addColumn<int>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("normal") << 5 << 5;
    QTest::newRow("erroneusUpper") << 66000 << 65535;
    QTest::newRow("erroneusLower") << -2 << 0;
}

void Tests::row_setDisplayIndex() {

    QFETCH(int, input);
    QFETCH(int, expected);

    Row c = Row();
    c.setDisplayIndex(input);
    QCOMPARE(c.displayIndex(), expected);
}

void Tests::row_setDisplayIndex_data() {
    QTest::addColumn<int>("input");
    QTest::addColumn<int>("expected");

    QTest::newRow("normal") << 5 << 5;
    QTest::newRow("erroneusUpper") << 66000 << 65535;
    QTest::newRow("erroneusLower") << -2 << 0;
}

void Tests::row_setName() {
    QFETCH(QString, input);
    QFETCH(QString, expected);

    Row c = Row();
    c.setName(input.toStdString());
    QCOMPARE(c.name().data(), expected.toStdString());
}

void Tests::row_setName_data() {
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

void Tests::row_setTotalValue() {
    QFETCH(float, input);
    QFETCH(float, expected);

    Row r = Row();
    r.setTotalValue(input);
    QCOMPARE(r.totalValue(), expected);
}

void Tests::row_setTotalValue_data() {
    QTest::addColumn<float>("input");
    QTest::addColumn<float>("expected");

    QTest::newRow("normalWhole") << 5.0f << 5.0f;
    QTest::newRow("normalDecimal") << 20.3f << 20.3f;
    QTest::newRow("erroneusUpper") << (float)(__FLT32_MAX__) << (float)(__FLT32_MAX__);
    QTest::newRow("erroneusLower") << (float)(__FLT32_MIN__) << (float)(__FLT32_MIN__);
}
