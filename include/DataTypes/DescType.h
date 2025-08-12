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

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        return 0.0f; // Description types don't have worth calculations
    }
};

#endif //DESCTYPE_H
