#include "Column.h"
#include <iostream>
#include <QtCore>

Column::Column(QString i_name, int i_importance, bool i_verbose)
{
    setName(i_name.toStdString());
    setImportance(i_importance);
    m_verbose = i_verbose;
    m_type = *DataType::createDataType(NAME);
    m_typeAutoSet = true;
}

void Column::setIndex(int i_index)
{
    if (i_index < 0)
    {
        m_trueIndex = 0;
        if (m_verbose) { std::cerr << "Warning: Attempted to set 'true index' to value lower than 0, clamping to 0\n"; }
        return;
    }
    if (i_index > 0xFFFF)
    {
        m_trueIndex = 0xFFFF;
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to set 'true index' to value higher than 0xFFFF, clamping to 0xFFFF\n";
        }
        return;
    }
    m_trueIndex = i_index;
}

int Column::index() const
{
    return m_trueIndex;
}

void Column::setDisplayIndex(int i_index)
{
    if (i_index < 0)
    {
        m_displayIndex = 0;
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to set 'display index' to value lower than 0, clamping to 0\n";
        }
        return;
    }
    if (i_index > 0xFFFF)
    {
        m_displayIndex = 0xFFFF;
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to set 'display index' to value higher than 0xFFFF, clamping to 0xFFFF\n";
        }
        return;
    }
    m_displayIndex = i_index;
}

int Column::displayIndex() const
{
    return m_displayIndex;
}

void Column::setName(std::string i_name)
{
    if (i_name.length() <= 0)
    {
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to assign blank name to 'column name', defaulting to 'name'\n";
        }
        m_name = "name";
        return;
    }

    for (int i = 0; i < i_name.length(); i++)
    {
        if (i_name[i] == '\n')
        {
            i_name[i] = ' ';
        }
    }

    std::string o = "";

    if (i_name[0] != ' ')
    {
        o.push_back(i_name[0]);
    }
    for (int i = 1; i < i_name.length() - 1; i++)
    {
        if (i_name[i] == ' ' && i_name[i + 1] == ' ')
        {
            i++;
        }
        o.push_back(i_name[i]);
    }
    if (i_name[i_name.length() - 1] != ' ')
    {
        o.push_back(i_name[i_name.length() - 1]);
    }
    m_name = o;
}

std::string Column::name() const
{
    return m_name;
}

void Column::setImportance(int i_importance)
{
    if (i_importance < -128)
    {
        m_importance = -128;
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to set 'importance' to value lower than -128, clamping to -128\n";
        }
        return;
    }
    if (i_importance > 127)
    {
        m_importance = 127;
        if (m_verbose)
        {
            std::cerr << "Warning: Attempted to set 'importance' to value higher than 127, clamping to 127\n";
        }
        return;
    }
    m_importance = i_importance;
}

int Column::importance() const
{
    return m_importance;
}

bool Column::operator==(const Column& i_column) const
{
    return (
        m_name == i_column.m_name &&
        m_importance == i_column.m_importance
    );
}

void Column::print() const
{
    std::cout << "Column Index: " << index() << '\n'
        << "Display Index: " << displayIndex() << '\n'
        << "Name: " << name() << '\n'
        << "Importance: " << importance() << '\n';
}

void Column::setType(DataType i_type)
{
    m_type = i_type;
    m_typeAutoSet = false;
}

DataType Column::type() const
{
    return m_type;
}

void Column::testAutoSetType(std::string_view i_value)
{
    if (m_typeAutoSet)
    {
        if (i_value == "") { return; }
        for (int i = 1; i < 7; i++)
        {
            if (DataType::createDataType((Type)i)->attemptAutoSet(i_value.data()))
            {
                m_type = *DataType::createDataType(static_cast<Type>(i));
                return;
            }
        }
    }
}

void Column::addBoundsValue(int32_t i_value) {
m_boundsValues.push_back(i_value);
	_sortBoundsValues();
}

void Column::removeBoundsValue(int i_index) {
for (int i = 0; i < m_boundsValues.size(); i++) {
	if (i > i_index) {
		m_boundsValues[i - 1] = m_boundsValues[i];
		}
	}
}

void Column::clearBoundsValues() {
	m_boundsValues.clear();
}

int32_t Column::boundsValue(int i_index) {
	if (i_index >= m_boundsValues.size())
	{
		std::cerr << "Attempted to get a bounds value with an index greater than the size of the vector";
		return 0;
	}
return m_boundsValues[i_index];
}

size_t Column::boundsValuesLength() {
	return m_boundsValues.size();
}

void Column::_sortBoundsValues() {
	for (int i = 0; i < m_boundsValues.size(); i++) {
		for (int j = 0; j < m_boundsValues.size() - 1; j++) {
			if (m_boundsValues[j] > m_boundsValues[j + 1])
			{
				auto temp = m_boundsValues[j];
				m_boundsValues[j] = m_boundsValues[j + 1];
				m_boundsValues[j + 1] = temp;
			}
		}
	}
}
