#ifndef TABLE_H
#define TABLE_H

#include "row.h"
#include <functional>
#include <stdlib.h>
#include <vector>
#include <string>
#include "column.h"

struct Item {
    std::string displayValue;
    float worthValue;
};

class Table
{
protected:
    std::vector<Column> m_headings = {};
    std::vector<Row> m_rows = {};
    std::vector<Item> m_items = {};
    std::string m_title = "Title";
    bool m_verbose = false;

public:
    Table();
    Table(bool i_verbose);
    ~Table() = default;

    void addHeading(Column i_heading);
    Column* heading(unsigned int i_index);
    unsigned int headingCount() const;
    void removeHeading(unsigned int i_index);
    int findHeading(std::function<bool(Column i_value)> i_predicate);
    void addRow(Row i_row);
    Row* row(unsigned int i_index);
    unsigned int rowCount() const;
    void removeRow(unsigned int i_index);
    int findRow(std::function<bool(Row i_value)> i_predicate);
    void setTitle(std::string i_title);
    std::string title() const;
    Item* item(unsigned int i_index);
    Item* item(unsigned int i_x, unsigned int i_y);
};

#endif // TABLE_H
