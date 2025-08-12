//
// Created by t on 15/05/25.
//

#ifndef LINKTYPE_H
#define LINKTYPE_H
#include "DataType.h"

class LinkType : public DataType
{
public:
    LinkType() : DataType()
    {
        m_name = "Link";
        m_desc = "Link to a website";
        m_enumType = Type::LINK;
        m_defaultImportance = 0;
    }

    [[nodiscard]] bool attemptAutoSet(std::string i_item) const override
    {
        return i_item.find("http://") == 0 || i_item.find("https://") == 0 || i_item.find("www.") == 0;
    }

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        return 0.0f; // Link types don't have worth calculations
    }
};

#endif //LINKTYPE_H
