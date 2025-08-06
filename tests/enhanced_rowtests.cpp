#include "test_main.h"
#include "Row.h"
#include <cfloat>

// Enhanced Row Tests - additional functionality
void Tests::row_constructor() {
    QFETCH(QString, name);
    QFETCH(bool, verbose);
    QFETCH(QString, expectedName);
    
    Row r(name, verbose);
    QCOMPARE(r.name(), expectedName.toStdString());
    QCOMPARE(r.index(), 0); // Default index
    QCOMPARE(r.displayIndex(), 0); // Default display index
    QCOMPARE(r.totalValue(), 0.0f); // Default total value
}

void Tests::row_constructor_data() {
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("verbose");
    QTest::addColumn<QString>("expectedName");
    
    // Pass cases
    QTest::newRow("normal name") << "Test Row" << false << "Test Row";
    QTest::newRow("verbose true") << "Verbose Row" << true << "Verbose Row";
    QTest::newRow("simple name") << "Simple" << false << "Simple";
    
    // Boundary cases
    QTest::newRow("empty name") << "" << false << "name"; // Default name handling
    QTest::newRow("long name") << QString("Very long row name that contains many characters and should be handled properly") << false << "Very long row name that contains many characters and should be handled properly";
    
    // Erroneous cases (should be handled gracefully)
    QTest::newRow("name with newlines") << "Row\nWith\nNewlines" << false << "Row With Newlines";
    QTest::newRow("name with tabs") << "Row\tWith\tTabs" << false << "Row\tWith\tTabs"; // Tabs not converted
    QTest::newRow("name with mixed whitespace") << "Row  \n\t  With   Mixed" << false << "Row  \t With  Mixed"; // Newlines to spaces, double spaces reduced
}

void Tests::row_equality() {
    QFETCH(Row, row1);
    QFETCH(Row, row2);
    QFETCH(bool, expected);
    
    QCOMPARE(row1 == row2, expected);
}

void Tests::row_equality_data() {
    QTest::addColumn<Row>("row1");
    QTest::addColumn<Row>("row2");
    QTest::addColumn<bool>("expected");
    
    // Pass cases - equal rows
    Row r1("Test Row");
    Row r2("Test Row");
    QTest::newRow("identical rows") << r1 << r2 << true;
    
    Row r3;
    Row r4;
    QTest::newRow("default rows") << r3 << r4 << true;
    
    // Boundary cases - slightly different rows
    Row r5("Test Row");
    Row r6("Different Row");
    QTest::newRow("different names") << r5 << r6 << false;
    
    Row r7("Same Name");
    r7.setTotalValue(10.5f);
    Row r8("Same Name");
    r8.setTotalValue(15.2f);
    QTest::newRow("same name different values") << r7 << r8 << true; // Row equality only compares names
    
    // Erroneous cases - completely different rows
    Row r9("Row A");
    r9.setIndex(5);
    r9.setTotalValue(100.0f);
    Row r10("Row B");
    r10.setIndex(10);
    r10.setTotalValue(-50.0f);
    QTest::newRow("completely different") << r9 << r10 << false;
}

void Tests::row_indexBoundaries() {
    QFETCH(int, inputIndex);
    QFETCH(int, expectedIndex);
    QFETCH(bool, isDisplayIndex);
    
    Row r;
    if (isDisplayIndex) {
        r.setDisplayIndex(inputIndex);
        QCOMPARE(r.displayIndex(), expectedIndex);
    } else {
        r.setIndex(inputIndex);
        QCOMPARE(r.index(), expectedIndex);
    }
}

void Tests::row_indexBoundaries_data() {
    QTest::addColumn<int>("inputIndex");
    QTest::addColumn<int>("expectedIndex");
    QTest::addColumn<bool>("isDisplayIndex");
    
    // Pass cases - normal indices
    QTest::newRow("normal index") << 100 << 100 << false;
    QTest::newRow("normal display index") << 200 << 200 << true;
    QTest::newRow("zero index") << 0 << 0 << false;
    QTest::newRow("zero display index") << 0 << 0 << true;
    
    // Boundary cases - at limits
    QTest::newRow("max uint16 index") << 65535 << 65535 << false;
    QTest::newRow("max uint16 display") << 65535 << 65535 << true;
    
    // Erroneous cases - beyond limits (should be clamped)
    QTest::newRow("over max index") << 70000 << 65535 << false;
    QTest::newRow("over max display") << 70000 << 65535 << true;
    QTest::newRow("negative index") << -1 << 0 << false;
    QTest::newRow("negative display") << -1 << 0 << true;
    QTest::newRow("very negative index") << -1000 << 0 << false;
    QTest::newRow("very negative display") << -1000 << 0 << true;
}

void Tests::row_totalValueBoundaries() {
    QFETCH(float, inputValue);
    QFETCH(float, expectedValue);
    
    Row r;
    r.setTotalValue(inputValue);
    QCOMPARE(r.totalValue(), expectedValue);
}

void Tests::row_totalValueBoundaries_data() {
    QTest::addColumn<float>("inputValue");
    QTest::addColumn<float>("expectedValue");
    
    // Pass cases - normal values
    QTest::newRow("positive value") << 10.5f << 10.5f;
    QTest::newRow("negative value") << -5.2f << FLT_MIN; // Negative values get clamped to FLT_MIN
    QTest::newRow("zero value") << 0.0f << 0.0f;
    QTest::newRow("small decimal") << 0.001f << 0.001f;
    
    // Boundary cases - extreme values
    QTest::newRow("max float") << FLT_MAX << FLT_MAX;
    QTest::newRow("min float") << FLT_MIN << FLT_MIN;
    QTest::newRow("lowest float") << -FLT_MAX << FLT_MIN; // Negative values get clamped to FLT_MIN
    QTest::newRow("epsilon") << FLT_EPSILON << FLT_EPSILON;
    
    // Erroneous cases - special float values
    QTest::newRow("infinity") << std::numeric_limits<float>::infinity() << FLT_MAX; // Infinity gets clamped to FLT_MAX
    QTest::newRow("negative infinity") << -std::numeric_limits<float>::infinity() << FLT_MIN; // Negative infinity gets clamped to FLT_MIN
    // Note: NaN test would be tricky with QCOMPARE, skipping for now
}

void Tests::row_nameEdgeCases() {
    QFETCH(QString, input);
    QFETCH(QString, expected);
    
    Row r;
    r.setName(input.toStdString());
    QCOMPARE(r.name(), expected.toStdString());
}

void Tests::row_nameEdgeCases_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");
    
    // Pass cases - normal names
    QTest::newRow("simple name") << "SimpleRow" << "SimpleRow";
    QTest::newRow("name with spaces") << "Row With Spaces" << "Row With Spaces";
    QTest::newRow("name with numbers") << "Row123" << "Row123";
    
    // Boundary cases - edge inputs
    QTest::newRow("very long name") << QString("R").repeated(1000) << QString("R").repeated(1000);
    QTest::newRow("unicode characters") << "Röw with ünicödé" << "Röw with ünicödé";
    QTest::newRow("special characters") << "Row!@#$%^&*()" << "Row!@#$%^&*()";
    
    // Erroneous cases - problematic inputs
    QTest::newRow("only whitespace") << "   \t\n   " << " \t  "; // Leading space stripped, newline to space, trailing spaces stripped
    QTest::newRow("mixed control chars") << "Row\a\b\f\v" << "Row\a\b\f\v"; // Only newlines converted
    QTest::newRow("leading/trailing spaces") << "  Row Name  " << " Row Name "; // Leading/trailing spaces preserved
}