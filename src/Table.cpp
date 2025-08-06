#include "Table.h"
#include <iostream>
#include <algorithm>

void Table::addHeading(Column i_column) {
    i_column.setIndex(headingCount());
    i_column.setDisplayIndex(i_column.index());

    m_headings.push_back(i_column);

    // Resize m_items to accommodate the new heading
    m_items.resize(headingCount() * rowCount());

    for (int x = 0; x < headingCount(); x++) {
        for (int y = 0; y < rowCount(); y++) {
            if (x == i_column.index()) {
                m_items[x * rowCount() + y] = Item();
            }
        }
    }
}

unsigned int Table::headingCount() const {
    return m_headings.size();
}

Column* Table::heading(unsigned int i_index) {
    if (i_index >= headingCount()) {
        if (m_verbose) {std::cerr << "Attempted to access 'heading' outside of array length, returning NullPtr\n";}
        return nullptr;
    }
    return &(m_headings[i_index]);
}

Table::Table() {
    // Column c;
    // c.setName("Blank Column");
    // addHeading(c);
    Row r;
    r.setName("Blank Name");
    addRow(r);
    m_items.resize(headingCount() * rowCount());
}

void Table::print() const {
    std::cout << "Table: " << m_title << "\n";
    for (const auto& heading : m_headings) {
        std::cout << heading.name() << "\t";
    }
    std::cout << "\n";

    for (const auto& row : m_rows) {
        std::cout << row.name() << "\t";
        for (unsigned int x = 0; x < headingCount(); x++) {
            std::cout << m_items[x * rowCount() + row.index()].displayValue << "\t";
        }
        std::cout << "\n";
    }
}

Table::Table(bool i_verbose) {
    m_verbose = i_verbose;
    // Column c;
    // c.setName("Blank Column");
    // addHeading(c);
    Row r;
    r.setName("Blank Name");
    addRow(r);
    m_items.resize(headingCount() * rowCount());
}

unsigned int Table::rowCount() const {
    return m_rows.size();
}

Item* Table::item(unsigned int i_x, unsigned int i_y) {
    auto hc = headingCount();
    if (i_x >= hc) {
        if (m_verbose) {std::cerr << "Attempted to access 'TotalValue', returnning NullPtr\n"; }
        return nullptr;
    }
    if (i_x * rowCount() + i_y > rowCount() * headingCount()) {
        if (m_verbose) {std::cerr << "Attempted to access 'item' outside of array length on dimension '" << (i_x > headingCount() ? "x" : "y") << "', returning NullPtr\n"; }
        return nullptr;
    }
    return &m_items[i_x * rowCount() + i_y];
}

void Table::removeHeading(unsigned int i_index) {
    std::vector<Column> o_c;
    std::vector<Item> o_i;
    for (int x = 0; x < headingCount(); x++) {
        if (x == i_index) {
            x++;
            if (x >= headingCount())
            {
                break;
            }
        }
        o_c.push_back(m_headings[x]);
        for (int y = 0; y < rowCount(); y++) {
            o_i.push_back(m_items[y * headingCount() + x]);
        }
    }
    m_items = o_i;
    m_headings = o_c;
}

void Table::addRow(Row i_row) {
    i_row.setIndex(rowCount());
    i_row.setDisplayIndex(i_row.index());

    m_rows.push_back(i_row);

    // Resize m_items to accommodate the new row
    m_items.resize(headingCount() * rowCount());

    for (int x = 0; x < headingCount(); x++) {
        for (int y = 0; y < rowCount(); y++) {
            if (y == i_row.index()) {
                m_items[x * rowCount() + y] = Item();
            }
        }
    }
}

void Table::removeRow(unsigned int i_index) {
    if (i_index >= rowCount()) {
        if (m_verbose) {
            std::cerr << "Attempted to remove a row outside of array bounds, operation aborted.\n";
        }
        return;
    }

    // Remove the row from m_rows
    m_rows.erase(m_rows.begin() + i_index);

    // Create a new vector for m_items
    std::vector<Item> o_i;
    o_i.reserve((rowCount() - 1) * headingCount());

    // Copy all items except those in the removed row
    for (unsigned int x = 0; x < headingCount(); x++) {
        for (unsigned int y = 0; y < rowCount() + 1; y++) {
            if (y != i_index) {
                o_i.push_back(m_items[y * headingCount() + x]);
            }
        }
    }

    // Update m_items
    m_items = std::move(o_i);
}

int Table::findRow(std::function<bool(Row)> i_predicate) {
    for (int i = 0; i < rowCount(); i++) {
        if (i_predicate(m_rows[i])) {
            return i;
        }
    }
    return -1;
}

int Table::findHeading(std::function<bool(Column)> i_predicate) {
    for (int i = 0; i < headingCount(); i++) {
        if (i_predicate(m_headings[i])) {
            return i;
        }
    }
    return -1;
}

Row* Table::row(unsigned int i_index) {
    if (i_index > rowCount()) {
        if (m_verbose) {std::cerr << "Attempted to access 'row' outside of array length, returning NullPtr\n";}
        return nullptr;
    }
    return &(m_rows[i_index]);
}

void Table::setTitle(std::string i_title) {
    if (i_title.length() <= 0) {
        if (m_verbose) {std::cerr << "Warning: Attempted to assign blank name to 'column name', defaulting to 'Title'\n";}
        m_title = "Title";
        return;
    }

    for (int i = 0; i < i_title.length(); i++) {
        if (i_title[i] == '\n') {
            i_title[i] = ' ';
        }
    }

    std::string o = "";

    if (i_title[0] != ' ') {
        o.push_back(i_title[0]);
    }
    for (int i = 1; i < i_title.length() - 1; i++) {
        if (i_title[i] == ' ' && i_title[i + 1] == ' ') {
            i++;
        }
        o.push_back(i_title[i]);
    }
    if (i_title[i_title.length() - 1] != ' ') {
        o.push_back(i_title[i_title.length() - 1]);
    }

    m_title = o;
}

std::string Table::title() const {
    return m_title;
}

void Table::calculateTotal(unsigned int i_row) {
    Row* r = row(i_row);
    float final = 0;
    for (int x = 0; x < headingCount(); x++) {
        Item* i = item(x, i_row);
        Column* h = heading(x);
        final += (i->worthValue * h->importance());
    }
    r->setTotalValue(final);
}

void Table::calculateAllTotals() {
    for (int y = 0; y < rowCount(); y++) {
        calculateTotal(y);
    }
}

Table Table::fromJson(const QJsonDocument& i_json)
{
    auto columns = i_json.object().value("columns").toArray();
    auto rows = i_json.object().value("rows").toArray();
    auto title = i_json.object().value("title").toString();
    auto table = Table();
    table.setTitle(title.toStdString());
    for (const auto& column : columns) {
        auto col = column.toObject();
        Column c;
        c.setName(col.value("name").toString().toStdString());
        c.setImportance(col.value("importance").toDouble());
        table.addHeading(c);
    }
    for (const auto& row : rows) {
        auto r = row.toObject();
        Row c;
        c.setName(r.value("name").toString().toStdString());
        table.addRow(c);
    }
    auto items = i_json.object().value("items").toArray();
    for (int i = 0; i < items.size(); i++) {
        auto item = items[i].toObject();
        auto x = item.value("x").toInt();
        auto y = item.value("y").toInt();
        auto displayValue = item.value("displayValue").toString().toStdString();
        auto worthValue = item.value("worthValue").toDouble();
        table.item(x, y)->displayValue = displayValue;
        table.item(x, y)->worthValue = worthValue;
    }
    return table;
}

void Table::sortByColumn(int columnIndex, bool ascending) {
    if (columnIndex < 0 || columnIndex >= static_cast<int>(headingCount()) + 2) {
        return; // Invalid column index
    }
    
    // Create a vector of row indices for sorting
    std::vector<int> rowIndices(rowCount());
    for (int i = 0; i < rowCount(); i++) {
        rowIndices[i] = i;
    }
    
    // Sort the indices based on the column values
    std::sort(rowIndices.begin(), rowIndices.end(), [this, columnIndex, ascending](int a, int b) {
        if (columnIndex == 0) {
            // Sort by row name
            std::string nameA = m_rows[a].name();
            std::string nameB = m_rows[b].name();
            return ascending ? nameA < nameB : nameA > nameB;
        } else if (columnIndex == static_cast<int>(headingCount()) + 1) {
            // Sort by total value
            float valueA = m_rows[a].totalValue();
            float valueB = m_rows[b].totalValue();
            return ascending ? valueA < valueB : valueA > valueB;
        } else {
            // Sort by column data
            int colIdx = columnIndex - 1;
            if (colIdx < 0 || colIdx >= static_cast<int>(headingCount())) return false;
            
            Item* itemA = item(colIdx, a);
            Item* itemB = item(colIdx, b);
            if (!itemA || !itemB) return false;
            
            // Try to compare as numbers first, then as strings
            try {
                float valueA = std::stof(itemA->displayValue);
                float valueB = std::stof(itemB->displayValue);
                return ascending ? valueA < valueB : valueA > valueB;
            } catch (...) {
                // Fall back to string comparison
                return ascending ? itemA->displayValue < itemB->displayValue : itemA->displayValue > itemB->displayValue;
            }
        }
    });
    
    // Reorder rows and items based on sorted indices
    std::vector<Row> newRows;
    std::vector<Item> newItems(headingCount() * rowCount());
    
    for (int newPos = 0; newPos < rowCount(); newPos++) {
        int oldPos = rowIndices[newPos];
        newRows.push_back(m_rows[oldPos]);
        
        // Copy items for this row
        for (int col = 0; col < headingCount(); col++) {
            int oldIndex = col * rowCount() + oldPos;
            int newIndex = col * rowCount() + newPos;
            if (oldIndex < m_items.size() && newIndex < newItems.size()) {
                newItems[newIndex] = m_items[oldIndex];
            }
        }
    }
    
    m_rows = newRows;
    m_items = newItems;
}

void Table::reorderColumnsByDisplayIndex() {
    // Create a vector of column indices sorted by displayIndex
    std::vector<int> columnIndices(headingCount());
    for (int i = 0; i < headingCount(); i++) {
        columnIndices[i] = i;
    }
    
    std::sort(columnIndices.begin(), columnIndices.end(), [this](int a, int b) {
        return m_headings[a].displayIndex() < m_headings[b].displayIndex();
    });
    
    // Reorder columns
    std::vector<Column> newHeadings;
    std::vector<Item> newItems(headingCount() * rowCount());
    
    for (int newCol = 0; newCol < headingCount(); newCol++) {
        int oldCol = columnIndices[newCol];
        newHeadings.push_back(m_headings[oldCol]);
        
        // Copy items for this column
        for (int row = 0; row < rowCount(); row++) {
            int oldIndex = oldCol * rowCount() + row;
            int newIndex = newCol * rowCount() + row;
            if (oldIndex < m_items.size() && newIndex < newItems.size()) {
                newItems[newIndex] = m_items[oldIndex];
            }
        }
    }
    
    m_headings = newHeadings;
    m_items = newItems;
}

void Table::reorderRowsByDisplayIndex() {
    // Create a vector of row indices sorted by displayIndex
    std::vector<int> rowIndices(rowCount());
    for (int i = 0; i < rowCount(); i++) {
        rowIndices[i] = i;
    }
    
    std::sort(rowIndices.begin(), rowIndices.end(), [this](int a, int b) {
        return m_rows[a].displayIndex() < m_rows[b].displayIndex();
    });
    
    // Reorder rows
    std::vector<Row> newRows;
    std::vector<Item> newItems(headingCount() * rowCount());
    
    for (int newPos = 0; newPos < rowCount(); newPos++) {
        int oldPos = rowIndices[newPos];
        newRows.push_back(m_rows[oldPos]);
        
        // Copy items for this row
        for (int col = 0; col < headingCount(); col++) {
            int oldIndex = col * rowCount() + oldPos;
            int newIndex = col * rowCount() + newPos;
            if (oldIndex < m_items.size() && newIndex < newItems.size()) {
                newItems[newIndex] = m_items[oldIndex];
            }
        }
    }
    
    m_rows = newRows;
    m_items = newItems;
}

