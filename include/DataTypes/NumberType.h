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

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        if (boundsValues.empty()) {
            return 0.0f;
        }

        float value = std::stof(displayValue);
        float worthValue = 0.0f;
        
        if (value < boundsValues[0]) {
            worthValue = ((0.0f / max) - 0.5f) * max;
            return worthValue;  // Original had break here
        }
        
        for (size_t j = 0; j < boundsValues.size() - 1; j++) {
            if (value >= boundsValues[j] && value < boundsValues[j + 1]) {
                worthValue = ((static_cast<float>(j) / max) - 0.5f) * max;
            }
        }
        
        if (value >= boundsValues[boundsValues.size() - 1]) {
            worthValue = ((static_cast<float>(boundsValues.size()) / max) - 0.5f) * max;
        }
        
        return worthValue;
    }
};

#endif //NUMBERTYPE_H
