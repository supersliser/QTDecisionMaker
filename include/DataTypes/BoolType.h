//
// Created by t on 15/05/25.
//

#ifndef BOOLTYPE_H
#define BOOLTYPE_H

#include "DataType.h"

class BoolType : public DataType
{
public:
    BoolType() : DataType()
    {
        m_name = "Bool";
        m_desc = "True or False";
        m_enumType = Type::BOOL;
        m_defaultImportance = 0;
    }

    [[nodiscard]] bool attemptAutoSet(std::string i_item) const override
    {
        return _toLower(i_item) == "true" || _toLower(i_item) == "false" || i_item == "1" || i_item == "0";
    }

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        if (displayValue == "True" || displayValue == "true" || displayValue == "1") {
            return 1.0f;
        } else {
            return -1.0f;
        }
    }

private:
    static std::string_view _toLower(std::string& i_str)
    {
        for (auto& c : i_str)
        {
            c = std::tolower(c);
        }
        return i_str;
    }
};

#endif //BOOLTYPE_H
