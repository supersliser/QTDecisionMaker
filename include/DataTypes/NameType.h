//
// Created by t on 15/05/25.
//

#ifndef NAMETYPE_H
#define NAMETYPE_H

#include <utility>

#include "DataType.h"

class NameType : public DataType<sts::string>
{
public:

    bool isValid(std::string value) const override {
        return !value.empty();
    }
};

#endif //NAMETYPE_H
