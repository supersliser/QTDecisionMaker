//
// Created for QTDecisionMaker preferences system
//

#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include <QObject>
#include <QStringList>
#include <QColor>
#include <QJsonObject>
#include <QApplication>
#include <QPalette>

class PreferencesManager : public QObject
{
    Q_OBJECT

public:
    explicit PreferencesManager(QObject* parent = nullptr);
    ~PreferencesManager() override = default;

    // Recent files management
    void addRecentFile(const QString& filePath);
    QStringList getRecentFiles() const;
    void clearRecentFiles();

    // Color preferences
    void setBackgroundColor(const QColor& color);
    QColor getBackgroundColor() const;
    void setTextColor(const QColor& color);
    QColor getTextColor() const;

    // Zoom preferences
    void setDefaultZoom(float zoom);
    float getDefaultZoom() const;

    // Save/Load preferences
    void savePreferences();
    void loadPreferences();

    // Reset to defaults
    void resetToDefaults();

    static QString getPreferencesFilePath();

signals:
    void backgroundColorChanged(const QColor& color);
    void textColorChanged(const QColor& color);
    void defaultZoomChanged(float zoom);

private:
    QStringList _recentFiles;
    QColor _backgroundColor;
    QColor _textColor;
    float _defaultZoom;

    static const int MAX_RECENT_FILES = 10;
    static const QString PREFERENCES_FILE_NAME;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);
    void initializeDefaults();
};

#endif // PREFERENCESMANAGER_H