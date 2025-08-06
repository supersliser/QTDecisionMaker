#include "test_main.h"
#include "PreferencesManager.h"
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QDir>
#include <QSignalSpy>

// PreferencesManager Tests
void Tests::preferencesmanager_recentFiles() {
    QFETCH(QStringList, inputFiles);
    QFETCH(QStringList, expectedFiles);
    QFETCH(int, maxFiles);
    
    PreferencesManager prefs;
    prefs.clearRecentFiles();
    
    // Add files one by one
    for (const QString& file : inputFiles) {
        prefs.addRecentFile(file);
    }
    
    QStringList result = prefs.getRecentFiles();
    
    // Check that we don't exceed max files (should be 10 by default)
    QVERIFY(result.size() <= 10);
    
    // Check the first expectedFiles.size() entries match
    for (int i = 0; i < qMin(expectedFiles.size(), result.size()); ++i) {
        QCOMPARE(result[i], expectedFiles[i]);
    }
}

void Tests::preferencesmanager_recentFiles_data() {
    QTest::addColumn<QStringList>("inputFiles");
    QTest::addColumn<QStringList>("expectedFiles");
    QTest::addColumn<int>("maxFiles");
    
    // Pass cases
    QStringList singleFile = {"file1.txt"};
    QTest::newRow("single file") << singleFile << singleFile << 10;
    
    QStringList multipleFiles = {"file1.txt", "file2.txt", "file3.txt"};
    QStringList expectedMultiple = {"file3.txt", "file2.txt", "file1.txt"}; // Most recent first
    QTest::newRow("multiple files") << multipleFiles << expectedMultiple << 10;
    
    // Boundary cases - test duplicate handling
    QStringList withDuplicates = {"file1.txt", "file2.txt", "file1.txt"};
    QStringList expectedDuplicates = {"file1.txt", "file2.txt"}; // Duplicate moved to front
    QTest::newRow("duplicate files") << withDuplicates << expectedDuplicates << 10;
    
    // Erroneous cases - exceed max files
    QStringList manyFiles;
    QStringList expectedManyFiles;
    for (int i = 0; i < 15; ++i) {
        manyFiles << QString("file%1.txt").arg(i);
        if (i >= 5) { // Only last 10 should remain
            expectedManyFiles.prepend(QString("file%1.txt").arg(i));
        }
    }
    QTest::newRow("exceed max files") << manyFiles << expectedManyFiles << 10;
}

void Tests::preferencesmanager_colors() {
    QFETCH(QColor, inputBackgroundColor);
    QFETCH(QColor, inputTextColor);
    QFETCH(QColor, expectedBackgroundColor);
    QFETCH(QColor, expectedTextColor);
    
    PreferencesManager prefs;
    
    // Set up signal spies
    QSignalSpy backgroundSpy(&prefs, &PreferencesManager::backgroundColorChanged);
    QSignalSpy textSpy(&prefs, &PreferencesManager::textColorChanged);
    
    // Set colors
    prefs.setBackgroundColor(inputBackgroundColor);
    prefs.setTextColor(inputTextColor);
    
    // Verify colors
    QCOMPARE(prefs.getBackgroundColor(), expectedBackgroundColor);
    QCOMPARE(prefs.getTextColor(), expectedTextColor);
    
    // Verify signals were emitted
    QCOMPARE(backgroundSpy.count(), 1);
    QCOMPARE(textSpy.count(), 1);
    
    // Verify signal parameters
    QList<QVariant> backgroundArgs = backgroundSpy.takeFirst();
    QList<QVariant> textArgs = textSpy.takeFirst();
    QCOMPARE(backgroundArgs.at(0).value<QColor>(), expectedBackgroundColor);
    QCOMPARE(textArgs.at(0).value<QColor>(), expectedTextColor);
}

void Tests::preferencesmanager_colors_data() {
    QTest::addColumn<QColor>("inputBackgroundColor");
    QTest::addColumn<QColor>("inputTextColor");
    QTest::addColumn<QColor>("expectedBackgroundColor");
    QTest::addColumn<QColor>("expectedTextColor");
    
    // Pass cases
    QTest::newRow("standard colors") << QColor(Qt::white) << QColor(Qt::black) 
                                    << QColor(Qt::white) << QColor(Qt::black);
    QTest::newRow("custom RGB") << QColor(128, 64, 192) << QColor(255, 128, 0)
                               << QColor(128, 64, 192) << QColor(255, 128, 0);
    
    // Boundary cases
    QTest::newRow("extreme colors") << QColor(0, 0, 0) << QColor(255, 255, 255)
                                   << QColor(0, 0, 0) << QColor(255, 255, 255);
    QTest::newRow("transparent colors") << QColor(255, 0, 0, 128) << QColor(0, 255, 0, 64)
                                       << QColor(255, 0, 0, 128) << QColor(0, 255, 0, 64);
    
    // Erroneous cases
    QTest::newRow("invalid colors") << QColor() << QColor() << QColor() << QColor();
}

void Tests::preferencesmanager_zoom() {
    QFETCH(float, inputZoom);
    QFETCH(float, expectedZoom);
    QFETCH(bool, shouldEmitSignal);
    
    PreferencesManager prefs;
    
    QSignalSpy zoomSpy(&prefs, &PreferencesManager::defaultZoomChanged);
    
    prefs.setDefaultZoom(inputZoom);
    
    QCOMPARE(prefs.getDefaultZoom(), expectedZoom);
    
    if (shouldEmitSignal) {
        QCOMPARE(zoomSpy.count(), 1);
        QList<QVariant> args = zoomSpy.takeFirst();
        QCOMPARE(args.at(0).toFloat(), expectedZoom);
    }
}

void Tests::preferencesmanager_zoom_data() {
    QTest::addColumn<float>("inputZoom");
    QTest::addColumn<float>("expectedZoom");
    QTest::addColumn<bool>("shouldEmitSignal");
    
    // Pass cases
    QTest::newRow("normal zoom") << 1.5f << 1.5f << true;
    QTest::newRow("min zoom") << 0.1f << 0.1f << true;
    QTest::newRow("max zoom") << 5.0f << 5.0f << true;
    
    // Boundary cases  
    QTest::newRow("default zoom") << 1.0f << 1.0f << true;
    QTest::newRow("very small zoom") << 0.01f << 0.01f << true;
    QTest::newRow("very large zoom") << 10.0f << 10.0f << true;
    
    // Erroneous cases
    QTest::newRow("zero zoom") << 0.0f << 0.0f << true;
    QTest::newRow("negative zoom") << -1.0f << -1.0f << true; // May be allowed
}

void Tests::preferencesmanager_defaults() {
    PreferencesManager prefs;
    
    // Get current values
    QColor originalBg = prefs.getBackgroundColor();
    QColor originalText = prefs.getTextColor();
    float originalZoom = prefs.getDefaultZoom();
    QStringList originalFiles = prefs.getRecentFiles();
    
    // Modify values
    prefs.setBackgroundColor(Qt::red);
    prefs.setTextColor(Qt::blue);
    prefs.setDefaultZoom(2.0f);
    prefs.addRecentFile("test.txt");
    
    // Verify they changed
    QVERIFY(prefs.getBackgroundColor() != originalBg);
    QVERIFY(prefs.getTextColor() != originalText);
    QVERIFY(prefs.getDefaultZoom() != originalZoom);
    QVERIFY(prefs.getRecentFiles() != originalFiles);
    
    // Reset to defaults
    prefs.resetToDefaults();
    
    // Verify defaults are restored (may not be exactly original values)
    QVERIFY(prefs.getBackgroundColor().isValid());
    QVERIFY(prefs.getTextColor().isValid());
    QVERIFY(prefs.getDefaultZoom() > 0.0f);
    QVERIFY(prefs.getRecentFiles().isEmpty());
}

void Tests::preferencesmanager_persistence() {
    QFETCH(QStringList, testFiles);
    QFETCH(QColor, testBgColor);
    QFETCH(QColor, testTextColor);
    QFETCH(float, testZoom);
    
    // Create first preferences manager and set values
    {
        PreferencesManager prefs1;
        prefs1.clearRecentFiles();
        
        for (const QString& file : testFiles) {
            prefs1.addRecentFile(file);
        }
        prefs1.setBackgroundColor(testBgColor);
        prefs1.setTextColor(testTextColor);
        prefs1.setDefaultZoom(testZoom);
        
        prefs1.savePreferences();
    } // prefs1 goes out of scope
    
    // Create second preferences manager and verify values are loaded
    {
        PreferencesManager prefs2;
        
        QStringList loadedFiles = prefs2.getRecentFiles();
        QColor loadedBgColor = prefs2.getBackgroundColor();
        QColor loadedTextColor = prefs2.getTextColor();
        float loadedZoom = prefs2.getDefaultZoom();
        
        // Verify persistence (accounting for max files limit)
        int expectedSize = qMin(testFiles.size(), 10); // MAX_RECENT_FILES is 10
        QCOMPARE(loadedFiles.size(), expectedSize);
        for (int i = 0; i < expectedSize; ++i) {
            int originalIndex = testFiles.size() - 1 - i; // Most recent first
            QCOMPARE(loadedFiles[i], testFiles[originalIndex]);
        }
        QCOMPARE(loadedBgColor, testBgColor);
        QCOMPARE(loadedTextColor, testTextColor);
        QCOMPARE(loadedZoom, testZoom);
    }
}

void Tests::preferencesmanager_persistence_data() {
    QTest::addColumn<QStringList>("testFiles");
    QTest::addColumn<QColor>("testBgColor");
    QTest::addColumn<QColor>("testTextColor");
    QTest::addColumn<float>("testZoom");
    
    // Pass cases
    QStringList files1 = {"file1.txt", "file2.txt"};
    QTest::newRow("basic persistence") << files1 << QColor(Qt::cyan) << QColor(Qt::magenta) << 1.8f;
    
    // Boundary cases
    QStringList noFiles;
    QTest::newRow("empty files") << noFiles << QColor(0, 0, 0) << QColor(255, 255, 255) << 0.5f;
    
    // Erroneous cases
    QStringList manyFiles;
    for (int i = 0; i < 20; ++i) {
        manyFiles << QString("file%1.txt").arg(i);
    }
    QTest::newRow("many files") << manyFiles << QColor(128, 128, 128) << QColor(64, 192, 64) << 3.0f;
}

void Tests::preferencesmanager_filePath() {
    QString path = PreferencesManager::getPreferencesFilePath();
    
    // Pass cases - verify path is valid
    QVERIFY(!path.isEmpty());
    QVERIFY(path.endsWith("preferences.json"));
    QVERIFY(QDir::isAbsolutePath(path));
    
    // Boundary cases - verify parent directory exists or can be created
    QFileInfo fileInfo(path);
    QDir parentDir = fileInfo.dir();
    if (!parentDir.exists()) {
        QVERIFY(parentDir.mkpath(parentDir.absolutePath()));
    }
    
    // Erroneous cases - verify we can write to the location
    QFile testFile(path + ".test");
    QVERIFY(testFile.open(QIODevice::WriteOnly));
    testFile.write("test");
    testFile.close();
    QVERIFY(testFile.remove());
}

void Tests::preferencesmanager_clearRecentFiles() {
    PreferencesManager prefs;
    
    // Clear any existing files first
    prefs.clearRecentFiles();
    
    // Add some files
    prefs.addRecentFile("file1.txt");
    prefs.addRecentFile("file2.txt");
    prefs.addRecentFile("file3.txt");
    
    // Verify files were added
    QVERIFY(!prefs.getRecentFiles().isEmpty());
    QCOMPARE(prefs.getRecentFiles().size(), 3);
    
    // Clear files again
    prefs.clearRecentFiles();
    
    // Verify files were cleared
    QVERIFY(prefs.getRecentFiles().isEmpty());
    QCOMPARE(prefs.getRecentFiles().size(), 0);
}