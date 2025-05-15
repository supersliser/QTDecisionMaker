//
// Created by t on 15/05/25.
//

#ifndef NAMETYPE_H
#define NAMETYPE_H

#include "DataType.h"

class NameType : public DataType
{
public:
    NameType() : DataType()
    {
        name = "Name";
        desc = "A name";
        enumType = NAME;
        defaultImportance = 0;
    }


};

#endif //NAMETYPE_H
