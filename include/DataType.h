//
// Created by t on 15/05/25.
//

#ifndef DATATYPE_H
#define DATATYPE_H
#include <functional>
#include <string>

#include "DataTypes/NameType.h"

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

template<typename baseType>
class DataType {
protected:
    std::string name;
    std::string desc;

    std::function<bool(baseType)> autoSetCondition;
    int defaultImportance;

public:
    DataType(std::string name, std::string desc, std::function<bool(baseType)> autoSetCondition, int defaultImportance)
            : name(name), desc(desc), autoSetCondition(autoSetCondition), defaultImportance(defaultImportance) {}

    virtual ~DataType() = default;

    static DataType * createDataType() {
        switch (type) {
        case NAME:
            return new NameType("Name", "Stores the name of an option in a decision", [](std::string item) {return false;}, 0);
        }
    }

    virtual bool isValid(baseType value) const = 0;

    virtual std::string getName() const {
        return name;
    }

    virtual std::string getDesc() const {
        return desc;
    }

    virtual int getDefaultImportance() const {
        return defaultImportance;
    }

    virtual bool autoSet(baseType value) const {
        return autoSetCondition(value);
    }
};



#endif //DATATYPE_H
