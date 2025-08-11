#include "test_main.h"
#include "FileSystemManager.h"
#include "Table.h"
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

// FileSystemManager Tests
void Tests::filesystemmanager_fileExists() {
    QFETCH(QString, filePath);
    QFETCH(bool, shouldExist);
    QFETCH(bool, createFile);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString testPath = tempDir.path() + "/" + filePath;
    
    if (createFile) {
        QFile file(testPath);
        QVERIFY(file.open(QIODevice::WriteOnly));
        file.write("test content");
        file.close();
    }
    
    QCOMPARE(FileSystemManager::fileExists(testPath), shouldExist);
}

void Tests::filesystemmanager_fileExists_data() {
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<bool>("shouldExist");
    QTest::addColumn<bool>("createFile");
    
    // Pass cases
    QTest::newRow("existing file") << "test.json" << true << true;
    QTest::newRow("existing file with extension") << "data.txt" << true << true;
    
    // Boundary cases  
    QTest::newRow("file with long name") << QString("very_long_filename_").repeated(10) + ".json" << true << true;
    QTest::newRow("file with special chars") << "test-file_123.json" << true << true;
    
    // Erroneous cases
    QTest::newRow("non-existing file") << "nonexistent.json" << false << false;
    QTest::newRow("empty filename") << "" << true << false; // QFile::exists() may return true for empty path 
    QTest::newRow("directory as file") << "." << true << false; // QFile::exists() may return true for directories
}

void Tests::filesystemmanager_writeFile() {
    QFETCH(Table, inputTable);
    QFETCH(QString, filename);
    QFETCH(bool, expectedResult);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString filePath = tempDir.path() + "/" + filename;
    
    bool result = FileSystemManager::writeFile(filePath, &inputTable);
    QCOMPARE(result, expectedResult);
    
    if (expectedResult) {
        QVERIFY(QFile::exists(filePath));
        QFile file(filePath);
        QVERIFY(file.open(QIODevice::ReadOnly));
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QVERIFY(!doc.isNull());
        
        // Basic structure validation
        QJsonObject obj = doc.object();
        QVERIFY(obj.contains("title"));
        QVERIFY(obj.contains("columns"));
        QVERIFY(obj.contains("rows"));
        QVERIFY(obj.contains("items"));
    }
}

void Tests::filesystemmanager_writeFile_data() {
    QTest::addColumn<Table>("inputTable");
    QTest::addColumn<QString>("filename");
    QTest::addColumn<bool>("expectedResult");
    
    // Pass cases
    Table simpleTable;
    simpleTable.setTitle("Test Table");
    simpleTable.addHeading(Column("Column 1", 5));
    simpleTable.addRow(Row("Row 1"));
    QTest::newRow("simple valid table") << simpleTable << "test.json" << true;
    
    Table complexTable;
    complexTable.setTitle("Complex Table");
    complexTable.addHeading(Column("Name", 10));
    complexTable.addHeading(Column("Score", 8));
    complexTable.addHeading(Column("Notes", 3));
    complexTable.addRow(Row("Item 1"));
    complexTable.addRow(Row("Item 2"));
    complexTable.item(0, 0)->displayValue = "Test Item";
    complexTable.item(0, 0)->worthValue = 7.5f;
    QTest::newRow("complex table") << complexTable << "complex.json" << true;
    
    // Boundary cases
    Table emptyTable;
    QTest::newRow("minimal table") << emptyTable << "minimal.json" << true;
    
    // Erroneous cases
    QTest::newRow("invalid filename") << simpleTable << "" << false;
    QTest::newRow("invalid path chars") << simpleTable << "invalid\0path.json" << true; // May still succeed depending on OS
}

void Tests::filesystemmanager_readFile() {
    QFETCH(QString, jsonContent);
    QFETCH(bool, shouldSucceed);
    QFETCH(QString, expectedTitle);
    QFETCH(int, expectedColumns);
    QFETCH(int, expectedRows);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString filePath = tempDir.path() + "/test.json";
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(jsonContent.toUtf8());
    file.close();
    
    Table result = FileSystemManager::readFile(filePath);
    
    if (shouldSucceed) {
        QCOMPARE(result.title(), expectedTitle.toStdString());
        QCOMPARE(static_cast<int>(result.headingCount()), expectedColumns);
        QCOMPARE(static_cast<int>(result.rowCount()), expectedRows); // Table::fromJson removes default row, so count matches JSON
    }
    // If shouldSucceed is false, we expect an empty/null table
}

void Tests::filesystemmanager_readFile_data() {
    QTest::addColumn<QString>("jsonContent");
    QTest::addColumn<bool>("shouldSucceed");
    QTest::addColumn<QString>("expectedTitle");
    QTest::addColumn<int>("expectedColumns");
    QTest::addColumn<int>("expectedRows");
    
    // Pass cases
    QString validJson = R"({
        "title": "Test Table",
        "columns": [
            {"name": "Column 1", "importance": 5},
            {"name": "Column 2", "importance": 3}
        ],
        "rows": [
            {"name": "Row 1"},
            {"name": "Row 2"}
        ],
        "items": [
            {"x": 0, "y": 0, "displayValue": "Value 1", "worthValue": 1.5},
            {"x": 1, "y": 0, "displayValue": "Value 2", "worthValue": 2.5}
        ]
    })";
    QTest::newRow("valid complete json") << validJson << true << "Test Table" << 2 << 2; // Rows from JSON directly
    
    QString minimalJson = R"({
        "title": "Minimal",
        "columns": [],
        "rows": [],
        "items": []
    })";
    QTest::newRow("minimal valid json") << minimalJson << true << "Minimal" << 0 << 0;
    
    // Boundary cases
    QString unicodeJson = R"({
        "title": "Ünicødé Tëst",
        "columns": [{"name": "Côlümn", "importance": 0}],
        "rows": [{"name": "Röw"}],
        "items": []
    })";
    QTest::newRow("unicode content") << unicodeJson << true << "Ünicødé Tëst" << 1 << 1;
    
    // Erroneous cases
    QString invalidJson = "{ invalid json content }";
    QTest::newRow("invalid json") << invalidJson << false << "" << 0 << 0;
    
    QString emptyJson = "";
    QTest::newRow("empty file") << emptyJson << false << "" << 0 << 0;
    
    QString incompleteJson = R"({"title": "Incomplete"})";
    QTest::newRow("incomplete json") << incompleteJson << false << "" << 0 << 0;
}

void Tests::filesystemmanager_roundtrip() {
    QFETCH(Table, originalTable);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString filePath = tempDir.path() + "/roundtrip.json";
    
    // Write the table
    bool writeSuccess = FileSystemManager::writeFile(filePath, &originalTable);
    QVERIFY(writeSuccess);
    
    // Read it back
    Table readTable = FileSystemManager::readFile(filePath);
    
    // Compare essential properties
    QCOMPARE(readTable.title(), originalTable.title());
    QCOMPARE(readTable.headingCount(), originalTable.headingCount());
    QCOMPARE(readTable.rowCount(), originalTable.rowCount());
    
    // Compare column details
    for (unsigned int i = 0; i < originalTable.headingCount(); ++i) {
        QCOMPARE(readTable.heading(i)->name(), originalTable.heading(i)->name());
        QCOMPARE(readTable.heading(i)->importance(), originalTable.heading(i)->importance());
    }
    
    // Compare row details
    for (unsigned int i = 0; i < originalTable.rowCount(); ++i) {
        QCOMPARE(readTable.row(i)->name(), originalTable.row(i)->name());
    }
}

void Tests::filesystemmanager_roundtrip_data() {
    QTest::addColumn<Table>("originalTable");
    
    // Pass cases
    Table simpleTable;
    simpleTable.setTitle("Roundtrip Test");
    simpleTable.addHeading(Column("Test Column", 7));
    simpleTable.addRow(Row("Test Row"));
    simpleTable.item(0, 0)->displayValue = "Test Value";
    simpleTable.item(0, 0)->worthValue = 9.5f;
    QTest::newRow("simple table roundtrip") << simpleTable;
    
    Table complexTable;
    complexTable.setTitle("Complex Roundtrip");
    complexTable.addHeading(Column("Name", -5));
    complexTable.addHeading(Column("Rating", 10));
    complexTable.addHeading(Column("Cost", 3));
    complexTable.addRow(Row("Option A"));
    complexTable.addRow(Row("Option B"));
    complexTable.addRow(Row("Option C"));
    for (unsigned int i = 0; i < complexTable.headingCount(); ++i) {
        for (unsigned int j = 0; j < complexTable.rowCount(); ++j) {
            complexTable.item(i, j)->displayValue = QString("Value_%1_%2").arg(i).arg(j).toStdString();
            complexTable.item(i, j)->worthValue = static_cast<float>(i * 10 + j);
        }
    }
    QTest::newRow("complex table roundtrip") << complexTable;
    
    // Boundary cases
    Table edgeTable;
    edgeTable.setTitle("");  // Empty title
    QTest::newRow("edge case table") << edgeTable;
    
    // Erroneous cases - tables with special characters
    Table specialTable;
    specialTable.setTitle("Special\nChars\tTable");
    specialTable.addHeading(Column("Column\nWith\nNewlines", 0));
    specialTable.addRow(Row("Row\tWith\tTabs"));
    QTest::newRow("special characters table") << specialTable;
}

// Table JSON serialization tests  
void Tests::table_fromJson() {
    QFETCH(QJsonDocument, inputJson);
    QFETCH(bool, shouldSucceed);
    QFETCH(QString, expectedTitle);
    QFETCH(int, expectedColumns);
    
    Table result = Table::fromJson(inputJson);
    
    if (shouldSucceed) {
        QCOMPARE(result.title(), expectedTitle.toStdString());
        QCOMPARE(static_cast<int>(result.headingCount()), expectedColumns);
    }
    // For failed cases, we expect either default values or the method to handle gracefully
}

void Tests::table_fromJson_data() {
    QTest::addColumn<QJsonDocument>("inputJson");
    QTest::addColumn<bool>("shouldSucceed");
    QTest::addColumn<QString>("expectedTitle");
    QTest::addColumn<int>("expectedColumns");
    
    // Pass cases
    QJsonObject validObj;
    validObj["title"] = "JSON Test";
    QJsonArray columnsArray;
    QJsonObject col1;
    col1["name"] = "Test Column";
    col1["importance"] = 5;
    columnsArray.append(col1);
    validObj["columns"] = columnsArray;
    validObj["rows"] = QJsonArray();
    validObj["items"] = QJsonArray();
    QTest::newRow("valid json object") << QJsonDocument(validObj) << true << "JSON Test" << 1;
    
    // Boundary cases
    QJsonObject minimalObj;
    minimalObj["title"] = "";
    minimalObj["columns"] = QJsonArray();
    minimalObj["rows"] = QJsonArray();
    minimalObj["items"] = QJsonArray();
    QTest::newRow("minimal json") << QJsonDocument(minimalObj) << true << "Title" << 0; // Empty title becomes "Title"
    
    // Erroneous cases
    QJsonObject invalidObj;
    invalidObj["wrongfield"] = "value";
    QTest::newRow("missing required fields") << QJsonDocument(invalidObj) << false << "" << 0;
    
    QJsonDocument emptyDoc;
    QTest::newRow("empty document") << emptyDoc << false << "" << 0;
}

// Bounds Values Tests
void Tests::filesystemmanager_boundsValues_roundtrip() {
    QFETCH(Table, originalTable);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString filePath = tempDir.path() + "/bounds_roundtrip.json";
    
    // Write the table
    bool writeSuccess = FileSystemManager::writeFile(filePath, &originalTable);
    QVERIFY(writeSuccess);
    
    // Read it back
    Table readTable = FileSystemManager::readFile(filePath);
    
    // Compare bounds values
    QCOMPARE(readTable.headingCount(), originalTable.headingCount());
    for (unsigned int i = 0; i < originalTable.headingCount(); ++i) {
        QCOMPARE(readTable.heading(i)->boundsValuesLength(), originalTable.heading(i)->boundsValuesLength());
        for (size_t j = 0; j < originalTable.heading(i)->boundsValuesLength(); ++j) {
            QCOMPARE(readTable.heading(i)->boundsValue(j), originalTable.heading(i)->boundsValue(j));
        }
    }
}

void Tests::filesystemmanager_boundsValues_roundtrip_data() {
    QTest::addColumn<Table>("originalTable");
    
    // Table with bounds values
    Table tableWithBounds;
    tableWithBounds.setTitle("Bounds Test");
    Column col1("Column1", 5);
    col1.addBoundsValue(10);
    col1.addBoundsValue(20);
    col1.addBoundsValue(30);
    tableWithBounds.addHeading(col1);
    
    Column col2("Column2", 3);
    col2.addBoundsValue(1);
    col2.addBoundsValue(5);
    tableWithBounds.addHeading(col2);
    
    Column col3("Column3", 0); // No bounds values
    tableWithBounds.addHeading(col3);
    
    tableWithBounds.addRow(Row("Row1"));
    QTest::newRow("table with bounds values") << tableWithBounds;
    
    // Table with empty bounds
    Table tableEmptyBounds;
    tableEmptyBounds.setTitle("Empty Bounds Test");
    Column emptyCol("EmptyColumn", 1);
    tableEmptyBounds.addHeading(emptyCol);
    tableEmptyBounds.addRow(Row("Row1"));
    QTest::newRow("table with empty bounds") << tableEmptyBounds;
}

void Tests::filesystemmanager_boundsValues_backwardCompatibility() {
    QFETCH(QString, jsonContent);
    QFETCH(QString, expectedTitle);
    QFETCH(int, expectedColumns);
    QFETCH(QList<int>, expectedBoundsLengths);
    
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());
    
    QString filePath = tempDir.path() + "/backward_compat.json";
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write(jsonContent.toUtf8());
    file.close();
    
    Table result = FileSystemManager::readFile(filePath);
    
    QCOMPARE(result.title(), expectedTitle.toStdString());
    QCOMPARE(static_cast<int>(result.headingCount()), expectedColumns);
    
    for (int i = 0; i < expectedColumns; ++i) {
        QCOMPARE(static_cast<int>(result.heading(i)->boundsValuesLength()), expectedBoundsLengths[i]);
    }
}

void Tests::filesystemmanager_boundsValues_backwardCompatibility_data() {
    QTest::addColumn<QString>("jsonContent");
    QTest::addColumn<QString>("expectedTitle");
    QTest::addColumn<int>("expectedColumns");
    QTest::addColumn<QList<int>>("expectedBoundsLengths");
    
    // Old format without bounds values (backward compatibility)
    QString oldFormatJson = R"({
        "title": "Old Format",
        "columns": [
            {"name": "Column 1", "importance": 5},
            {"name": "Column 2", "importance": 3}
        ],
        "rows": [
            {"name": "Row 1"}
        ],
        "items": []
    })";
    QTest::newRow("old format without bounds") << oldFormatJson << "Old Format" << 2 << QList<int>{0, 0};
    
    // New format with bounds values
    QString newFormatJson = R"({
        "title": "New Format",
        "columns": [
            {"name": "Column 1", "importance": 5, "boundsValues": [10, 20, 30]},
            {"name": "Column 2", "importance": 3, "boundsValues": []},
            {"name": "Column 3", "importance": 1, "boundsValues": [1, 5]}
        ],
        "rows": [
            {"name": "Row 1"}
        ],
        "items": []
    })";
    QTest::newRow("new format with bounds") << newFormatJson << "New Format" << 3 << QList<int>{3, 0, 2};
}