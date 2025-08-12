#include "test_main.h"
#include "Column.h"
#include "DataTypes/NumberType.h"
#include "DataTypes/RatingType.h"
#include "DataTypes/MoneyType.h"
#include "DataTypes/BoolType.h"
#include <cfloat>

// Worth Calculation Tests - comprehensive testing of autoCalculateWorth functionality
void Tests::worthcalculation_numType() {
    QFETCH(QString, displayValue);
    QFETCH(QVector<int32_t>, boundsValues);
    QFETCH(int, max);
    QFETCH(float, expectedWorth);
    
    NumType numType;
    std::vector<int32_t> bounds;
    for (int32_t value : boundsValues) {
        bounds.push_back(value);
    }
    
    float actualWorth = numType.autoCalculateWorth(displayValue.toStdString(), bounds, max);
    QCOMPARE(actualWorth, expectedWorth);
}

void Tests::worthcalculation_numType_data() {
    QTest::addColumn<QString>("displayValue");
    QTest::addColumn<QVector<int32_t>>("boundsValues");
    QTest::addColumn<int>("max");
    QTest::addColumn<float>("expectedWorth");
    
    // Basic cases
    QTest::newRow("value in first range") << "1" << QVector<int32_t>{1, 2, 3} << 3 << -1.5f;
    QTest::newRow("value in middle range") << "2" << QVector<int32_t>{1, 2, 3} << 3 << -0.5f;
    QTest::newRow("value at upper bound") << "3" << QVector<int32_t>{1, 2, 3} << 3 << 1.5f;
    
    // Edge cases
    QTest::newRow("value below first bound") << "0" << QVector<int32_t>{1, 2, 3} << 3 << -1.5f;
    QTest::newRow("value above last bound") << "4" << QVector<int32_t>{1, 2, 3} << 3 << 1.5f;
    QTest::newRow("empty bounds") << "5" << QVector<int32_t>{} << 3 << 0.0f;
    QTest::newRow("single bound") << "1" << QVector<int32_t>{1} << 1 << 0.5f;
    
    // Fractional values
    QTest::newRow("fractional value in range") << "1.5" << QVector<int32_t>{1, 2, 3} << 3 << -1.5f;
    QTest::newRow("fractional value exact bound") << "2.0" << QVector<int32_t>{1, 2, 3} << 3 << -0.5f;
}

void Tests::worthcalculation_ratingType() {
    QFETCH(QString, displayValue);
    QFETCH(QVector<int32_t>, boundsValues);
    QFETCH(int, max);
    QFETCH(float, expectedWorth);
    
    RateType rateType(5, 1); // Rating 1-5
    std::vector<int32_t> bounds;
    for (int32_t value : boundsValues) {
        bounds.push_back(value);
    }
    
    float actualWorth = rateType.autoCalculateWorth(displayValue.toStdString(), bounds, max);
    QCOMPARE(actualWorth, expectedWorth);
}

void Tests::worthcalculation_ratingType_data() {
    QTest::addColumn<QString>("displayValue");
    QTest::addColumn<QVector<int32_t>>("boundsValues");
    QTest::addColumn<int>("max");
    QTest::addColumn<float>("expectedWorth");
    
    // Basic rating cases
    QTest::newRow("rating 1 of 5") << "1" << QVector<int32_t>{1, 3, 5} << 3 << -1.5f;
    QTest::newRow("rating 3 of 5") << "3" << QVector<int32_t>{1, 3, 5} << 3 << -0.5f;
    QTest::newRow("rating 5 of 5") << "5" << QVector<int32_t>{1, 3, 5} << 3 << 1.5f;
    
    // Edge cases for ratings
    QTest::newRow("rating below min") << "0" << QVector<int32_t>{1, 3, 5} << 3 << -1.5f;
    QTest::newRow("rating above max") << "6" << QVector<int32_t>{1, 3, 5} << 3 << 1.5f;
}

void Tests::worthcalculation_polymorphism() {
    QFETCH(Type, dataType);
    QFETCH(QString, displayValue);
    QFETCH(QVector<int32_t>, boundsValues);
    QFETCH(int, max);
    QFETCH(bool, shouldReturnNonZero);
    
    // Create column and set the datatype via pointer to test polymorphism
    Column column;
    DataType* dt = DataType::createDataType(dataType);
    column.setType(dt);
    
    // Set bounds values
    column.clearBoundsValues();
    for (int32_t value : boundsValues) {
        column.addBoundsValue(value);
    }
    
    // Get the worth calculation through the column's type reference
    float worth = column.type().autoCalculateWorth(displayValue.toStdString(), 
                                                   column.boundsValues(), max);
    
    if (shouldReturnNonZero) {
        QVERIFY(worth != 0.0f);
    } else {
        QCOMPARE(worth, 0.0f);
    }
}

void Tests::worthcalculation_polymorphism_data() {
    QTest::addColumn<Type>("dataType");
    QTest::addColumn<QString>("displayValue");
    QTest::addColumn<QVector<int32_t>>("boundsValues");
    QTest::addColumn<int>("max");
    QTest::addColumn<bool>("shouldReturnNonZero");
    
    // Test that polymorphism works correctly for different types
    QTest::newRow("NUM type calculation") << NUM << "2" << QVector<int32_t>{1, 2, 3} << 3 << true;
    QTest::newRow("RATE type calculation") << RATE << "3" << QVector<int32_t>{1, 3, 5} << 3 << true;
    QTest::newRow("NAME type default") << NAME << "test" << QVector<int32_t>{1, 2, 3} << 3 << false;
    QTest::newRow("DESC type default") << DESC << "description" << QVector<int32_t>{1, 2, 3} << 3 << false;
    QTest::newRow("BOOL type special") << BOOL << "true" << QVector<int32_t>{1, 2, 3} << 3 << true;
}

void Tests::worthcalculation_boundsIntegration() {
    QFETCH(QVector<int32_t>, initialBounds);
    QFETCH(int32_t, addValue);
    QFETCH(int, removeIndex);
    QFETCH(QString, testValue);
    QFETCH(float, expectedWorth);
    
    Column column;
    NumType* numType = new NumType();
    column.setType(numType);
    
    // Set initial bounds
    for (int32_t value : initialBounds) {
        column.addBoundsValue(value);
    }
    
    // Add a value
    if (addValue != -999) { // Use -999 as sentinel for "don't add"
        column.addBoundsValue(addValue);
    }
    
    // Remove a value
    if (removeIndex >= 0) {
        column.removeBoundsValue(removeIndex);
    }
    
    // Calculate worth
    float worth = column.type().autoCalculateWorth(testValue.toStdString(), 
                                                   column.boundsValues(), 
                                                   column.boundsValuesLength());
    
    QCOMPARE(worth, expectedWorth);
}

void Tests::worthcalculation_boundsIntegration_data() {
    QTest::addColumn<QVector<int32_t>>("initialBounds");
    QTest::addColumn<int32_t>("addValue");
    QTest::addColumn<int>("removeIndex");
    QTest::addColumn<QString>("testValue");
    QTest::addColumn<float>("expectedWorth");
    
    // Test bounds management integration
    QTest::newRow("add bound and test") << QVector<int32_t>{1, 3} << 2 << -1 << "2" << -0.5f;
    QTest::newRow("remove bound and test") << QVector<int32_t>{1, 2, 3} << -999 << 1 << "2" << -1.5f;
    QTest::newRow("clear and rebuild") << QVector<int32_t>{} << 5 << -1 << "5" << 0.5f;
}

void Tests::worthcalculation_edgeCases() {
    QFETCH(QString, displayValue);
    QFETCH(QVector<int32_t>, boundsValues);
    QFETCH(int, max);
    QFETCH(bool, expectsException);
    
    NumType numType;
    std::vector<int32_t> bounds;
    for (int32_t value : boundsValues) {
        bounds.push_back(value);
    }
    
    if (expectsException) {
        // Test that invalid inputs throw exceptions appropriately
        bool exceptionThrown = false;
        try {
            float worth = numType.autoCalculateWorth(displayValue.toStdString(), bounds, max);
            Q_UNUSED(worth);
        } catch (...) {
            exceptionThrown = true;
        }
        QVERIFY(exceptionThrown); // We expect an exception for invalid input
    } else {
        float worth = numType.autoCalculateWorth(displayValue.toStdString(), bounds, max);
        // Just verify it returns a valid float (allow infinity for division by zero)
        if (max == 0) {
            // Division by zero case - expect infinity or NaN
            QVERIFY(std::isinf(worth) || std::isnan(worth));
        } else {
            QVERIFY(!std::isnan(worth));
        }
    }
}

void Tests::worthcalculation_edgeCases_data() {
    QTest::addColumn<QString>("displayValue");
    QTest::addColumn<QVector<int32_t>>("boundsValues");
    QTest::addColumn<int>("max");
    QTest::addColumn<bool>("expectsException");
    
    // Edge cases that should be handled gracefully
    QTest::newRow("non-numeric string") << "abc" << QVector<int32_t>{1, 2, 3} << 3 << true;
    QTest::newRow("empty string") << "" << QVector<int32_t>{1, 2, 3} << 3 << true;
    QTest::newRow("zero max") << "2" << QVector<int32_t>{1, 2, 3} << 0 << false; // May cause infinity
    QTest::newRow("negative max") << "2" << QVector<int32_t>{1, 2, 3} << -1 << false;
    QTest::newRow("very large value") << "999999" << QVector<int32_t>{1, 2, 3} << 3 << false;
    QTest::newRow("negative value") << "-5" << QVector<int32_t>{1, 2, 3} << 3 << false;
    QTest::newRow("unsorted bounds") << "2" << QVector<int32_t>{3, 1, 2} << 3 << false;
}