#ifndef COLUMN_H
#define COLUMN_H

#include <cstdint>
#include <string>
#include <QtCore>


class Column
{
protected:
    uint16_t m_trueIndex = 0;
    uint16_t m_displayIndex = 0;
    std::string m_name = "name";
    int8_t m_importance = 0;
    bool m_verbose = false;
public:
    Column() = default;
    Column(QString i_name, int i_importance = 0, bool i_verbose = false);
    ~Column() = default;

    void setIndex(int i_index);
    int index() const;
    void setDisplayIndex(int i_dispIndex);
    int displayIndex() const;
    void setName(std::string i_name);
    std::string name() const;
    void setImportance(int i_importance);
    int importance() const;

    void print() const;

    bool operator==(const Column &i_column) const;
};

#endif // COLUMN_H
