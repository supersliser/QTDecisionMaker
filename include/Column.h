#ifndef COLUMN_H
#define COLUMN_H

#include <cstdint>
#include <string>
#include <QtCore>

#include "DataType.h"


class Column
{
protected:
    uint16_t m_trueIndex = 0;
    uint16_t m_displayIndex = 0;
    DataType m_type;
    bool m_typeAutoSet = true;
    std::string m_name = "name";
    int8_t m_importance = 0;
    bool m_verbose = false;
	std::vector<int32_t> m_boundsValues = {};

	void _sortBoundsValues();
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
    void setType(DataType i_type);
    DataType type() const;
    void testAutoSetType(std::string_view i_value);
	void addBoundsValue(int32_t i_value);
	void removeBoundsValue(int i_index);
	void clearBoundsValues();
	int32_t boundsValue(int i_index);
	size_t boundsValuesLength();
	void setBoundsValue(int i_index, int i_value);
    void print() const;

    bool operator==(const Column &i_column) const;
};

#endif // COLUMN_H
