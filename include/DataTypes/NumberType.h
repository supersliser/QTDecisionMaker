//
// Created by t on 15/05/25.
//

#ifndef NUMBERTYPE_H
#define NUMBERTYPE_H

#include <algorithm>

#include "DataType.h"

class NumType : public DataType
{
public:

    NumType() : DataType()
    {
        m_name = "Number";
        m_desc = "Generic Number";
        m_enumType = Type::NUM;
        m_defaultImportance = 0;
    }

    [[nodiscard]] bool attemptAutoSet(std::string i_item) const override
    {
        return std::all_of(i_item.begin(), i_item.end(), ::isdigit);
    }
};

#endif //NUMBERTYPE_H
