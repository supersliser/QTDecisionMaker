//
// Created by t on 15/05/25.
//

#ifndef LINKTYPE_H
#define LINKTYPE_H
#include "DataType.h"

class LinkType : public DataType
{
public:
    LinkType() : DataType()
    {
        name = "Link";
        desc = "Link to a website";
        defaultImportance = 0;
    }

    bool attemptAutoSet(std::string item) const override
    {
        return item.find("http://") == 0 || item.find("https://") == 0 || item.find("www.") == 0;
    }
};

#endif //LINKTYPE_H
