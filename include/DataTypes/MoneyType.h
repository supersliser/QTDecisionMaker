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
        name = "Money";
        desc = "Amount of money in a decision";
        defaultImportance = 0;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return item.find('$') == 0 || item.find("£") == 0 || item.find("€") == 0 ||
               item.find("¥") == 0 || item.find("₹") == 0 || item.find("₩") == 0 ||
               item.find("₽") == 0 || item.find("₺") == 0 || item.find("₫") == 0 ||
               item.find("₭") == 0 || item.find("₮") == 0 || item.find("₱") == 0 ||(item[item.find('.') + 1] == '0' && item[item.find('.') + 2] == '0') && std::all_of(item.begin() + 1, item.end(), ::isdigit);
    }
};

#endif //MONEYTYPE_H
