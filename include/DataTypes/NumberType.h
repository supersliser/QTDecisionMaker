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
        name = "Number";
        desc = "Generic Number";
        defaultImportance = 0;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return std::all_of(item.begin(), item.end(), ::isdigit);
    }
};

#endif //NUMBERTYPE_H
