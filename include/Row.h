#ifndef ROW_H
#define ROW_H
#include <cstdint>
#include <cstdlib>
#include <QtCore>
#include <string>

class Row
{
public:
    Row() = default;
    Row(QString i_name, bool i_verbose = false);

    void setIndex(int i_index);
    int index() const;
    void setDisplayIndex(int i_index);
    int displayIndex() const;
    void setName(std::string i_name);
    std::string name() const;
    void setTotalValue(float i_totalValue);
    float totalValue() const;

    bool operator==(const Row &i_row) const;

protected:
    uint16_t m_trueIndex = 0;
    uint16_t m_displayIndex = 0;
    std::string m_name = "name";
    float m_totalValue = 0;
    bool m_verbose = false;
};

#endif // ROW_H
