//
// Created by t on 15/05/25.
//

#ifndef RATINGTYPE_H
#define RATINGTYPE_H

#include <algorithm>

#include "DataType.h"

class RateType : public DataType
{
protected:
    int maxNumber;
    int minNumber;
public:

    explicit RateType(int i_maxNumber = 5, int i_minNumber = 0) : DataType()
    {
        m_name = "Rate";
        m_desc = "Rating of an option in a decision";
        m_enumType = Type::RATE;
        m_defaultImportance = 0;
        maxNumber = i_maxNumber;
        minNumber = i_minNumber;
    }

    [[nodiscard]] bool attemptAutoSet(std::string i_item) const override
    {
        return std::all_of(i_item.begin(), i_item.end(), ::isdigit) && std::stoi(i_item) >= minNumber && std::stoi(i_item) <= maxNumber;
    }

    [[nodiscard]] float autoCalculateWorth(const std::string& displayValue, 
                                           const std::vector<int32_t>& boundsValues, 
                                           int max) const override
    {
        if (boundsValues.empty()) {
            return 0.0f;
        }

        float value = std::stof(displayValue);
        
        if (value < boundsValues[0]) {
            return ((0.0f / max) - 0.5f) * max;
        }
        
        for (size_t j = 0; j < boundsValues.size() - 1; j++) {
            if (value >= boundsValues[j] && value < boundsValues[j + 1]) {
                return ((static_cast<float>(j) / max) - 0.5f) * max;
            }
        }
        
        if (value >= boundsValues[boundsValues.size() - 1]) {
            return ((static_cast<float>(boundsValues.size()) / max) - 0.5f) * max;
        }
        
        return 0.0f;
    }

    void setMaxNumber(int i_maxNumber)
    {
        maxNumber = i_maxNumber;
    }

    void setMinNumber(int i_minNumber)
    {
        minNumber = i_minNumber;
    }
};

#endif //RATINGTYPE_H
