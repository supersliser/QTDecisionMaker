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
    std::string m_name;
    std::string m_desc;
    Type m_enumType = NAME;
    int m_defaultImportance = 0;

public:
    DataType() = default;

    virtual ~DataType() = default;

    static DataType * createDataType(Type i_type);

    [[nodiscard]] virtual bool attemptAutoSet(std::string item) const {
        return false;
    }

    [[nodiscard]] virtual std::string name() const {
        return m_name;
    }

    [[nodiscard]] virtual std::string desc() const {
        return m_desc;
    }

    [[nodiscard]] virtual int defaultImportance() const {
        return m_defaultImportance;
    }

    [[nodiscard]] virtual Type type() const {
        return m_enumType;
    }
};



#endif //DATATYPE_H
