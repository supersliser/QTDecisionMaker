//
// Created by t on 14/05/25.
//

#ifndef FILESYSTEMMANAGER_H
#define FILESYSTEMMANAGER_H

#include <QJsonDocument>

#include "Table.h"


class FileSystemManager
{
public:
    static Table readFile(const QString& i_filePath);
    static bool writeFile(const QString& i_filePath, Table* i_jsonDoc);
    static bool fileExists(const QString& i_filePath);
};


#endif //FILESYSTEMMANAGER_H
