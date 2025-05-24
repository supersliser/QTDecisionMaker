//
// Created by t on 15/05/25.
//

#ifndef DESCTYPE_H
#define DESCTYPE_H

#include "DataType.h"

class DescType : public DataType
{
public:
    DescType(): DataType()
    {
        m_name = "Desc";
        m_desc = "Description of an option in a decision";
        m_enumType = Type::DESC;
        m_defaultImportance = 0;
    }

    bool attemptAutoSet(std::string i_item) const override
    {
        return i_item.length() > 30;
    }
};

#endif //DESCTYPE_H
