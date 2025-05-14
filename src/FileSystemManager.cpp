//
// Created by t on 14/05/25.
//

#include "../include/FileSystemManager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>

Table FileSystemManager::readFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading:" << file.errorString();
        return NULL;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (jsonDoc.isNull()) {
        qDebug() << "Failed to parse JSON document.";
    }

    return Table::fromJson(jsonDoc);
}

bool FileSystemManager::writeFile(const QString &filePath, Table *jsonDoc) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QJsonObject jsonObject;
    QJsonArray columnsArray;
    for (unsigned int i = 0; i < jsonDoc->headingCount(); ++i) {
        Column *column = jsonDoc->heading(i);
        QJsonObject columnObject;
        columnObject["name"] = QString::fromStdString(column->name());
        columnObject["importance"] = column->importance();
        columnsArray.append(columnObject);
    }
    jsonObject["columns"] = columnsArray;
    QJsonArray rowsArray;
    for (unsigned int i = 0; i < jsonDoc->rowCount(); ++i) {
        Row *row = jsonDoc->row(i);
        QJsonObject rowObject;
        rowObject["name"] = QString::fromStdString(row->name());
        QJsonArray itemsArray;
        for (unsigned int j = 0; j < jsonDoc->headingCount(); ++j) {
            Item *item = jsonDoc->item(j, i);
            QJsonObject itemObject;
            itemObject["displayValue"] = QString::fromStdString(item->displayValue);
            itemObject["worthValue"] = item->worthValue;
            itemsArray.append(itemObject);
        }
        rowObject["items"] = itemsArray;
        rowsArray.append(rowObject);
    }
    jsonObject["rows"] = rowsArray;
    QJsonArray itemsArray;
    for (int i = 0; i < jsonDoc->headingCount(); ++i) {
        for (int j = 0; j < jsonDoc->rowCount(); ++j) {
            Item *item = jsonDoc->item(i, j);
            QJsonObject itemObject;
            itemObject["x"] = i;
            itemObject["y"] = j;
            itemObject["displayValue"] = QString::fromStdString(item->displayValue);
            itemObject["worthValue"] = item->worthValue;
            itemsArray.append(itemObject);
        }
    }
    jsonObject["items"] = itemsArray;
    jsonObject["title"] = QString::fromStdString(jsonDoc->title());
    QJsonDocument jsonDocToWrite(jsonObject);
    if (jsonDocToWrite.isNull()) {
        qDebug() << "Failed to create JSON document.";
        return false;
    }
    if (file.write(jsonDocToWrite.toJson()) == -1) {
        qDebug() << "Failed to write JSON document to file:" << file.errorString();
        return false;
    }
    if (!file.flush()) {
        qDebug() << "Failed to flush file:" << file.errorString();
        return false;
    }
    file.close();
    return true;
}

bool FileSystemManager::fileExists(const QString &filePath) {
    QFile file(filePath);
    return file.exists();
}