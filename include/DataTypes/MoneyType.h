//
// Created by t on 15/05/25.
//

#ifndef MONEYTYPE_H
#define MONEYTYPE_H

#include <algorithm>

#include "DataType.h"

class MoneyType : public DataType
{
    public:

    MoneyType() : DataType()
    {
        m_name = "Money";
        m_desc = "Amount of money in a decision";
        m_enumType = Type::MONEY;
        m_defaultImportance = 0;
    }

    [[nodiscard]] bool attemptAutoSet(std::string i_item) const override
    {
        return i_item.find('$') == 0 || i_item.find("£") == 0 || i_item.find("€") == 0 ||
               i_item.find("¥") == 0 || i_item.find("₹") == 0 || i_item.find("₩") == 0 ||
               i_item.find("₽") == 0 || i_item.find("₺") == 0 || i_item.find("₫") == 0 ||
               i_item.find("₭") == 0 || i_item.find("₮") == 0 || i_item.find("₱") == 0 ||(i_item[i_item.find('.') + 1] == '0' && i_item[i_item.find('.') + 2] == '0') && std::all_of(i_item.begin() + 1, i_item.end(), ::isdigit);
    }
};

#endif //MONEYTYPE_H
