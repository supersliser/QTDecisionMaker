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
#include <stdio.h>
#include <iostream>

Table FileSystemManager::readFile(const QString &i_filePath) {
    QFile file(i_filePath);
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

bool FileSystemManager::writeFile(const QString &i_filePath, Table *i_jsonDoc) {
    QFile file(i_filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QJsonObject jsonObject;
    QJsonArray columnsArray;
    for (unsigned int i = 0; i < i_jsonDoc->headingCount(); ++i) {
        Column *column = i_jsonDoc->heading(i);
        QJsonObject columnObject;
        columnObject["name"] = QString::fromStdString(column->name());
        columnObject["importance"] = column->importance();
	columnObject["type"] = column->type().type();
        // Save bounds values
        QJsonArray boundsArray;
        for (size_t j = 0; j < column->boundsValuesLength(); ++j) {
            boundsArray.append(column->boundsValue(j));
        }
        columnObject["boundsValues"] = boundsArray;
        columnsArray.append(columnObject);
    }
    jsonObject["columns"] = columnsArray;
    QJsonArray rowsArray;
    for (unsigned int i = 0; i < i_jsonDoc->rowCount(); ++i) {
        Row *row = i_jsonDoc->row(i);
        QJsonObject rowObject;
        rowObject["name"] = QString::fromStdString(row->name());
        QJsonArray itemsArray;
        for (unsigned int j = 0; j < i_jsonDoc->headingCount(); ++j) {
            Item *item = i_jsonDoc->item(j, i);
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
    for (int i = 0; i < i_jsonDoc->headingCount(); ++i) {
        for (int j = 0; j < i_jsonDoc->rowCount(); ++j) {
            Item *item = i_jsonDoc->item(i, j);
            QJsonObject itemObject;
            itemObject["x"] = i;
            itemObject["y"] = j;
            itemObject["displayValue"] = QString::fromStdString(item->displayValue);
            itemObject["worthValue"] = item->worthValue;
            itemsArray.append(itemObject);
        }
    }
    jsonObject["items"] = itemsArray;
    jsonObject["title"] = QString::fromStdString(i_jsonDoc->title());
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

bool FileSystemManager::fileExists(const QString &i_filePath) {
    QFile file(i_filePath);
    return file.exists();
}
