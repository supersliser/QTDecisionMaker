//
// Created by t on 15/05/25.
//

#include "DataType.h"

#include "DataTypes/BoolType.h"
#include "DataTypes/DescType.h"
#include "DataTypes/LinkType.h"
#include "DataTypes/MoneyType.h"
#include "DataTypes/NameType.h"
#include "DataTypes/NumberType.h"
#include "DataTypes/RatingType.h"

DataType* DataType::createDataType(Type i_type)
{
    switch (i_type)
    {
        case NAME:
            return new NameType();
        case DESC:
            return new DescType();
        case LINK:
            return new LinkType();
        case BOOL:
            return new BoolType();
        case RATE:
            return new RateType();
        case MONEY:
            return new MoneyType();
        case NUM:
            return new NumType();
        default:
            return nullptr;
    }
}

DataType& DataType::operator=(const DataType &i_type)
{
    if (this != &i_type) {
        m_name = i_type.m_name;
        m_desc = i_type.m_desc;
        m_enumType = i_type.m_enumType;
        m_defaultImportance = i_type.m_defaultImportance;
    }
    return *this;
}