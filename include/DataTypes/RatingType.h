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

    RateType(int i_maxNumber = 5, int i_minNumber = 0) : DataType()
    {
        name = "Rate";
        desc = "Rating of an option in a decision";
        enumType = Type::RATE;
        defaultImportance = 0;
        maxNumber = i_maxNumber;
        minNumber = i_minNumber;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return std::all_of(item.begin(), item.end(), ::isdigit) && std::stoi(item) >= minNumber && std::stoi(item) <= maxNumber;
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
