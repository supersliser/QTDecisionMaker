//
// Created by t on 14/05/25.
//

#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include <QJsonDocument>

#include "table.h"


class FileSystemManager
{
public:
    static Table readFile(const QString& filePath);
    static bool writeFile(const QString& filePath, Table* jsonDoc);
    static bool fileExists(const QString& filePath);
};


#endif //FILESYSTEMMANAGER_H
