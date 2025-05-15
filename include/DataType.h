//
// Created by t on 15/05/25.
//

#ifndef DATATYPE_H
#define DATATYPE_H
#include <functional>
#include <string>

enum Type
{
    NAME,
    DESC,
    LINK,
    BOOL,
    RATE,
    MONEY,
    NUM
};

class DataType {
protected:
    std::string name;
    std::string desc;
    Type enumType;
    int defaultImportance = 0;

public:
    DataType() = default;

    virtual ~DataType() = default;

    static DataType * createDataType(Type type);

    virtual bool attemptAutoSet(std::string item) const {
        return false;
    }

    virtual std::string getName() const {
        return name;
    }

    virtual std::string getDesc() const {
        return desc;
    }

    virtual int getDefaultImportance() const {
        return defaultImportance;
    }

    virtual Type getType() const {
        return enumType;
    }
};



#endif //DATATYPE_H
