//
// Created for QTDecisionMaker preferences system
//

#include "../include/PreferencesManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

const QString PreferencesManager::PREFERENCES_FILE_NAME = "preferences.json";

PreferencesManager::PreferencesManager(QObject* parent)
    : QObject(parent)
    , _backgroundColor(Qt::white)
    , _textColor(Qt::black)
    , _defaultZoom(1.0f)
{
    loadPreferences();
}

void PreferencesManager::addRecentFile(const QString& filePath)
{
    // Remove if already in list
    _recentFiles.removeAll(filePath);
    
    // Add to front
    _recentFiles.prepend(filePath);
    
    // Keep only MAX_RECENT_FILES
    while (_recentFiles.size() > MAX_RECENT_FILES)
    {
        _recentFiles.removeLast();
    }
    
    savePreferences();
}

QStringList PreferencesManager::getRecentFiles() const
{
    return _recentFiles;
}

void PreferencesManager::clearRecentFiles()
{
    _recentFiles.clear();
    savePreferences();
}

void PreferencesManager::setBackgroundColor(const QColor& color)
{
    _backgroundColor = color;
    savePreferences();
}

QColor PreferencesManager::getBackgroundColor() const
{
    return _backgroundColor;
}

void PreferencesManager::setTextColor(const QColor& color)
{
    _textColor = color;
    savePreferences();
}

QColor PreferencesManager::getTextColor() const
{
    return _textColor;
}

void PreferencesManager::setDefaultZoom(float zoom)
{
    _defaultZoom = zoom;
    savePreferences();
}

float PreferencesManager::getDefaultZoom() const
{
    return _defaultZoom;
}

QString PreferencesManager::getPreferencesFilePath()
{
    QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataLocation);
    if (!dir.exists())
    {
        dir.mkpath(dataLocation);
    }
    return dir.filePath(PREFERENCES_FILE_NAME);
}

void PreferencesManager::savePreferences()
{
    QJsonDocument doc(toJson());
    QFile file(getPreferencesFilePath());
    
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
    else
    {
        qWarning() << "Failed to save preferences to:" << getPreferencesFilePath() << "-" << file.errorString();
    }
}

void PreferencesManager::loadPreferences()
{
    QFile file(getPreferencesFilePath());
    
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (!doc.isNull() && doc.isObject())
        {
            fromJson(doc.object());
        }
    }
    else
    {
        // File doesn't exist or can't be read, use defaults
        resetToDefaults();
    }
}

void PreferencesManager::resetToDefaults()
{
    _recentFiles.clear();
    _backgroundColor = Qt::white;
    _textColor = Qt::black;
    _defaultZoom = 1.0f;
    savePreferences();
}

QJsonObject PreferencesManager::toJson() const
{
    QJsonObject json;
    
    // Recent files
    QJsonArray recentArray;
    for (const QString& file : _recentFiles)
    {
        recentArray.append(file);
    }
    json["recentFiles"] = recentArray;
    
    // Colors
    json["backgroundColor"] = _backgroundColor.name();
    json["textColor"] = _textColor.name();
    
    // Zoom
    json["defaultZoom"] = _defaultZoom;
    
    return json;
}

void PreferencesManager::fromJson(const QJsonObject& json)
{
    // Recent files
    _recentFiles.clear();
    if (json.contains("recentFiles") && json["recentFiles"].isArray())
    {
        QJsonArray recentArray = json["recentFiles"].toArray();
        for (const QJsonValue& value : recentArray)
        {
            if (value.isString())
            {
                _recentFiles.append(value.toString());
            }
        }
    }
    
    // Colors
    if (json.contains("backgroundColor") && json["backgroundColor"].isString())
    {
        _backgroundColor.setNamedColor(json["backgroundColor"].toString());
    }
    if (json.contains("textColor") && json["textColor"].isString())
    {
        _textColor.setNamedColor(json["textColor"].toString());
    }
    
    // Zoom
    if (json.contains("defaultZoom") && json["defaultZoom"].isDouble())
    {
        _defaultZoom = static_cast<float>(json["defaultZoom"].toDouble());
    }
}