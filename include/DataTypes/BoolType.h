//
// Created by t on 15/05/25.
//

#ifndef BOOLTYPE_H
#define BOOLTYPE_H

#include "DataType.h"

class BoolType : public DataType
{
public:
    BoolType() : DataType()
    {
        name = "Bool";
        desc = "True or False";
        defaultImportance = 0;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return toLower(item) == "true" || toLower(item) == "false" || item == "1" || item == "0";
    }

private:
    static std::string_view toLower(std::string& str)
    {
        for (auto& c : str)
        {
            c = std::tolower(c);
        }
        return str;
    }
};

#endif //BOOLTYPE_H
