//
// Created by t on 15/05/25.
//

#ifndef MONEYTYPE_H
#define MONEYTYPE_H

#include <algorithm>
#include <iostream>
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

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        if (boundsValues.empty()) {
            return 0.0f;
        }
        float value = std::stof(displayValue);
        float worthValue = 0.0f;
        
        if (value <= boundsValues[boundsValues.size() - 1]) {
            worthValue = ((static_cast<float>(boundsValues.size()) / max) - 0.5f) * max;
        }
        // Preserve the original broken loop behavior for exact compatibility
        for (int j = static_cast<int>(boundsValues.size()) - 2; j >= 0; j--) {
            if (value <= boundsValues[j] && value > boundsValues[j + 1]) {
                worthValue = ((static_cast<float>(j) / max) - 0.5f) * max;
            }
        }
        if (value > boundsValues[0]) {
            worthValue = ((0.0f / max) - 0.5f) * max;
        }
        
        return worthValue;
    }
};

#endif //MONEYTYPE_H
