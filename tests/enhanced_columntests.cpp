#include "test_main.h"
#include "Column.h"
#include "DataTypes/BoolType.h"
#include "DataTypes/RatingType.h"
#include "DataTypes/MoneyType.h"

// Enhanced Column Tests - additional functionality
void Tests::column_constructor() {
    QFETCH(QString, name);
    QFETCH(int, importance);
    QFETCH(bool, verbose);
    QFETCH(QString, expectedName);
    QFETCH(int, expectedImportance);
    
    Column c(name, importance, verbose);
    QCOMPARE(c.name(), expectedName.toStdString());
    QCOMPARE(c.importance(), expectedImportance);
    QCOMPARE(c.index(), 0); // Default index
    QCOMPARE(c.displayIndex(), 0); // Default display index
}

void Tests::column_constructor_data() {
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("importance");
    QTest::addColumn<bool>("verbose");
    QTest::addColumn<QString>("expectedName");
    QTest::addColumn<int>("expectedImportance");
    
    // Pass cases
    QTest::newRow("normal values") << "Test Column" << 5 << false << "Test Column" << 5;
    QTest::newRow("verbose true") << "Verbose Column" << 10 << true << "Verbose Column" << 10;
    QTest::newRow("zero importance") << "Zero Importance" << 0 << false << "Zero Importance" << 0;
    
    // Boundary cases
    QTest::newRow("max importance") << "Max Importance" << 127 << false << "Max Importance" << 127;
    QTest::newRow("min importance") << "Min Importance" << -128 << false << "Min Importance" << -128;
    QTest::newRow("empty name") << "" << 5 << false << "name" << 5; // Default name handling
    
    // Erroneous cases (should be handled gracefully)
    QTest::newRow("over max importance") << "Over Max" << 300 << false << "Over Max" << 127;
    QTest::newRow("under min importance") << "Under Min" << -300 << false << "Under Min" << -128;
    QTest::newRow("special chars name") << "Test\nColumn\tWith\rSpecial" << 5 << false << "Test Column\tWith\rSpecial" << 5; // Only newlines get converted
}

void Tests::column_setType() {
    QFETCH(Type, inputType);
    QFETCH(QString, expectedTypeName);
    
    Column c;
    DataType* dt = DataType::createDataType(inputType);
    c.setType(*dt);
    QCOMPARE(c.type().name(), expectedTypeName.toStdString());
    QCOMPARE(c.type().type(), inputType);
    delete dt;
}

void Tests::column_setType_data() {
    QTest::addColumn<Type>("inputType");
    QTest::addColumn<QString>("expectedTypeName");
    
    QTest::newRow("NAME type") << NAME << "Name";
    QTest::newRow("BOOL type") << BOOL << "Bool";
    QTest::newRow("RATE type") << RATE << "Rate";
    QTest::newRow("MONEY type") << MONEY << "Money";
    QTest::newRow("NUM type") << NUM << "Number";
    QTest::newRow("DESC type") << DESC << "Desc";
    QTest::newRow("LINK type") << LINK << "Link";
}

void Tests::column_testAutoSetType() {
    QFETCH(QString, inputValue);
    QFETCH(Type, expectedType);
    
    Column c;
    c.testAutoSetType(inputValue.toStdString());
    QCOMPARE(c.type().type(), expectedType);
}

void Tests::column_testAutoSetType_data() {
    QTest::addColumn<QString>("inputValue");
    QTest::addColumn<Type>("expectedType");
    
    // Pass cases - should auto-detect types
    QTest::newRow("boolean true") << "true" << BOOL;
    QTest::newRow("boolean false") << "false" << BOOL;
    QTest::newRow("boolean 1") << "1" << BOOL;
    QTest::newRow("boolean 0") << "0" << BOOL;
    QTest::newRow("money dollar") << "$100" << MONEY;
    QTest::newRow("money pound") << "£50" << MONEY;
    QTest::newRow("rating 1-5") << "3" << RATE;
    QTest::newRow("rating 0") << "0" << BOOL; // "0" matches BOOL first in the auto-detection loop
    QTest::newRow("rating 5") << "5" << RATE;
    QTest::newRow("link http") << "http://example.com" << LINK;
    QTest::newRow("link https") << "https://example.com" << LINK;
    QTest::newRow("link www") << "www.example.com" << LINK;
    QTest::newRow("number") << "12345" << NUM;
    QTest::newRow("long description") << "This is a very long description that exceeds thirty characters" << DESC;
    
    // Boundary cases - ambiguous or edge cases
    QTest::newRow("exactly 31 chars") << "This is exactly thirty-one char" << DESC;
    QTest::newRow("exactly 30 chars") << "This is exactly thirty chars!!" << NAME; // Falls back to NAME
    
    // Erroneous cases - should default to NAME
    QTest::newRow("short text") << "short" << NAME;
    QTest::newRow("empty string") << "" << NAME;
    QTest::newRow("special chars") << "!@#$%^&*()" << NAME;
}

void Tests::column_equality() {
    QFETCH(Column, column1);
    QFETCH(Column, column2);
    QFETCH(bool, expected);
    
    QCOMPARE(column1 == column2, expected);
}

void Tests::column_equality_data() {
    QTest::addColumn<Column>("column1");
    QTest::addColumn<Column>("column2");
    QTest::addColumn<bool>("expected");
    
    // Pass cases - equal columns
    Column c1("Test", 5);
    Column c2("Test", 5);
    QTest::newRow("identical columns") << c1 << c2 << true;
    
    Column c3;
    Column c4;
    QTest::newRow("default columns") << c3 << c4 << true;
    
    // Boundary cases - slightly different columns
    Column c5("Test", 5);
    Column c6("Test", 6);
    QTest::newRow("different importance") << c5 << c6 << false;
    
    Column c7("Test1", 5);
    Column c8("Test2", 5);
    QTest::newRow("different names") << c7 << c8 << false;
    
    // Erroneous cases - very different columns  
    Column c9("Column A", 10);
    Column c10("Column B", -5);
    QTest::newRow("completely different") << c9 << c10 << false;
}

void Tests::column_indexBoundaries() {
    QFETCH(int, inputIndex);
    QFETCH(int, expectedIndex);
    QFETCH(bool, isDisplayIndex);
    
    Column c;
    if (isDisplayIndex) {
        c.setDisplayIndex(inputIndex);
        QCOMPARE(c.displayIndex(), expectedIndex);
    } else {
        c.setIndex(inputIndex);
        QCOMPARE(c.index(), expectedIndex);
    }
}

void Tests::column_indexBoundaries_data() {
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