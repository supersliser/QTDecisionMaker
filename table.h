#ifndef TABLE_H
#define TABLE_H

#include "column.h"
#include "row.h"
#include <stdlib.h>
#include <vector>
#include <string>

struct Item {
    std::string displayValue;
    float worthValue;
};

class Table
{
protected:
    std::vector<Column> m_headings;
    std::vector<Row> m_rows;
    std::vector<Item> m_items;
    std::string m_title;

public:
    Table() = default;
    ~Table() = default;

    void addHeading(Column i_heading);
    Column* heading(unsigned int i_index);
    void addRow(Row i_row);
    Row* row(unsigned int i_index);
    void setTitle(std::string i_title);
    std::string title() const;
    Item* item(unsigned int i_index);
    Item* item(unsigned int i_x, unsigned int i_y);
};

#endif // TABLE_H
