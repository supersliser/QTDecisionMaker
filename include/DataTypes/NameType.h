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
        m_name = "Name";
        m_desc = "A name";
        m_enumType = NAME;
        m_defaultImportance = 0;
    }


};

#endif //NAMETYPE_H
