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
        name = "Desc";
        desc = "Description of an option in a decision";
        enumType = Type::DESC;
        defaultImportance = 0;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return item.length() > 30;
    }
};

#endif //DESCTYPE_H
