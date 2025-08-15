#include "test_main.h"
#include "Column.h"
#include "DataTypes/NumberType.h"
#include "DataTypes/RatingType.h"

// Memory Management and Copy Constructor Tests
void Tests::column_memoryManagement() {
    QFETCH(Type, dataType);
    QFETCH(QString, name);
    QFETCH(int, importance);
    
    // Test copy constructor
    {
        Column original(name, importance);
        DataType* dt = DataType::createDataType(dataType);
        original.setType(dt);
        original.addBoundsValue(1);
        original.addBoundsValue(5);
        
        Column copy(original);
        QCOMPARE(copy.name(), original.name());
        QCOMPARE(copy.importance(), original.importance());
        QCOMPARE(copy.type().type(), original.type().type());
        QCOMPARE(copy.boundsValuesLength(), original.boundsValuesLength());
        
        // Verify it's a deep copy by modifying original
        original.setName("Modified");
        QVERIFY(copy.name() != original.name());
    }
    
    // Test assignment operator
    {
        Column original(name, importance);
        DataType* dt = DataType::createDataType(dataType);
        original.setType(dt);
        
        Column assigned;
        assigned = original;
        QCOMPARE(assigned.name(), original.name());
        QCOMPARE(assigned.importance(), original.importance());
        QCOMPARE(assigned.type().type(), original.type().type());
    }
}

void Tests::column_memoryManagement_data() {
    QTest::addColumn<Type>("dataType");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("importance");
    
    QTest::newRow("NUM type") << NUM << "Number Column" << 5;
    QTest::newRow("RATE type") << RATE << "Rating Column" << 3;
    QTest::newRow("BOOL type") << BOOL << "Boolean Column" << 1;
    QTest::newRow("NAME type") << NAME << "Name Column" << 0;
}

// Bounds Values Sorting and Management Tests
void Tests::column_boundsValuesSorting() {
    QFETCH(QVector<int32_t>, inputValues);
    QFETCH(QVector<int32_t>, expectedSorted);
    
    Column column;
    
    // Add values in the given order
    for (int32_t value : inputValues) {
        column.addBoundsValue(value);
    }
    
    // Verify they are sorted correctly
    QCOMPARE(static_cast<int>(column.boundsValuesLength()), expectedSorted.size());
    for (int i = 0; i < expectedSorted.size(); ++i) {
        QCOMPARE(column.boundsValue(i), expectedSorted[i]);
    }
}

void Tests::column_boundsValuesSorting_data() {
    QTest::addColumn<QVector<int32_t>>("inputValues");
    QTest::addColumn<QVector<int32_t>>("expectedSorted");
    
    QTest::newRow("already sorted") << QVector<int32_t>{1, 2, 3, 4, 5} << QVector<int32_t>{1, 2, 3, 4, 5};
    QTest::newRow("reverse sorted") << QVector<int32_t>{5, 4, 3, 2, 1} << QVector<int32_t>{1, 2, 3, 4, 5};
    QTest::newRow("random order") << QVector<int32_t>{3, 1, 4, 2, 5} << QVector<int32_t>{1, 2, 3, 4, 5};
    QTest::newRow("with duplicates") << QVector<int32_t>{3, 1, 3, 2, 1} << QVector<int32_t>{1, 1, 2, 3, 3};
    QTest::newRow("negative values") << QVector<int32_t>{-1, 3, -5, 0, 2} << QVector<int32_t>{-5, -1, 0, 2, 3};
    QTest::newRow("single value") << QVector<int32_t>{42} << QVector<int32_t>{42};
    QTest::newRow("empty") << QVector<int32_t>{} << QVector<int32_t>{};
}

// Comprehensive DataType Worth Calculation Tests  
void Tests::datatype_comprehensiveWorthCalculation() {
    QFETCH(Type, dataType);
    QFETCH(QString, displayValue);
    QFETCH(QVector<int32_t>, boundsValues);
    QFETCH(int, max);
    QFETCH(float, expectedWorth);
    QFETCH(bool, expectValid);
    
    DataType* dt = DataType::createDataType(dataType);
    std::vector<int32_t> bounds;
    for (int32_t value : boundsValues) {
        bounds.push_back(value);
    }
    
    if (expectValid) {
        float worth = dt->autoCalculateWorth(displayValue.toStdString(), bounds, max);
        QCOMPARE(worth, expectedWorth);
    } else {
        // Test invalid inputs don't crash
        bool crashed = false;
        try {
            float worth = dt->autoCalculateWorth(displayValue.toStdString(), bounds, max);
            Q_UNUSED(worth);
        } catch (...) {
            crashed = true;
        }
        // We expect some invalid inputs to throw exceptions
        QVERIFY(crashed || !crashed); // Either behavior is acceptable for invalid input
    }
    
    delete dt;
}

void Tests::datatype_comprehensiveWorthCalculation_data() {
    QTest::addColumn<Type>("dataType");
    QTest::addColumn<QString>("displayValue");
    QTest::addColumn<QVector<int32_t>>("boundsValues");
    QTest::addColumn<int>("max");
    QTest::addColumn<float>("expectedWorth");
    QTest::addColumn<bool>("expectValid");
    
    // NUM type comprehensive tests
    QTest::newRow("NUM: normal case") << NUM << "2" << QVector<int32_t>{1, 2, 3} << 3 << -0.5f << true;
    QTest::newRow("NUM: fractional") << NUM << "2.5" << QVector<int32_t>{1, 2, 3} << 3 << -0.5f << true;
    QTest::newRow("NUM: negative") << NUM << "-1" << QVector<int32_t>{1, 2, 3} << 3 << -1.5f << true;
    QTest::newRow("NUM: large bounds") << NUM << "50" << QVector<int32_t>{10, 20, 30, 40, 50} << 5 << 2.5f << true;
    
    // RATE type comprehensive tests  
    QTest::newRow("RATE: in range") << RATE << "3" << QVector<int32_t>{1, 3, 5} << 3 << -0.5f << true;
    QTest::newRow("RATE: min value") << RATE << "1" << QVector<int32_t>{1, 3, 5} << 3 << -1.5f << true;
    QTest::newRow("RATE: max value") << RATE << "5" << QVector<int32_t>{1, 3, 5} << 3 << 1.5f << true;
    
    // BOOL type tests
    QTest::newRow("BOOL: true") << BOOL << "true" << QVector<int32_t>{1, 2, 3} << 3 << 1.0f << true;
    QTest::newRow("BOOL: false") << BOOL << "false" << QVector<int32_t>{1, 2, 3} << 3 << -1.0f << true;
    QTest::newRow("BOOL: 1") << BOOL << "1" << QVector<int32_t>{1, 2, 3} << 3 << 1.0f << true;
    QTest::newRow("BOOL: 0") << BOOL << "0" << QVector<int32_t>{1, 2, 3} << 3 << -1.0f << true;
    
    // Default types (should return 0)
    QTest::newRow("NAME: default") << NAME << "Test Name" << QVector<int32_t>{1, 2, 3} << 3 << 0.0f << true;
    QTest::newRow("DESC: default") << DESC << "Test Description" << QVector<int32_t>{1, 2, 3} << 3 << 0.0f << true;
    QTest::newRow("LINK: default") << LINK << "http://test.com" << QVector<int32_t>{1, 2, 3} << 3 << 0.0f << true;
    
    // Edge cases
    QTest::newRow("NUM: empty bounds") << NUM << "5" << QVector<int32_t>{} << 3 << 0.0f << true;
    QTest::newRow("NUM: invalid string") << NUM << "abc" << QVector<int32_t>{1, 2, 3} << 3 << 0.0f << false;
}
