#include "row.h"
#include <iostream>

Row::Row(bool i_verbose) {
    m_verbose = i_verbose;
}

void Row::setIndex(int i_index) {
    if (i_index < 0) {
        m_trueIndex = 0;
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'true index' to value lower than 0, clamping to 0\n";}
        return;
    }
    if (i_index > 0xFFFF) {
        m_trueIndex = 0xFFFF;
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'true index' to value higher than 0xFFFF, clamping to 0xFFFF\n";}
        return;
    }
    m_trueIndex = i_index;
}

int Row::index() const {
    return m_trueIndex;
}

void Row::setDisplayIndex(int i_index) {
    if (i_index < 0) {
        m_displayIndex = 0;
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'display index' to value lower than 0, clamping to 0\n";}
        return;
    }
    if (i_index > 0xFFFF) {
        m_displayIndex = 0xFFFF;
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'display index' to value higher than 0xFFFF, clamping to 0xFFFF\n";}
        return;
    }
    m_displayIndex = i_index;
}

int Row::displayIndex() const {
    return m_displayIndex;
}

void Row::setName(std::string i_name) {
    if (i_name.length() <= 0) {
        m_name = "name";
        return;
    }

    for (int i = 0; i < i_name.length(); i++) {
        if (i_name[i] == '\n') {
            i_name[i] = ' ';
        }
    }

    std::string o = "";

    if (i_name[0] != ' ') {
        o.push_back(i_name[0]);
    }
    for (int i = 1; i < i_name.length() - 1; i++) {
        if (i_name[i] == ' ' && i_name[i + 1] == ' ') {
            i++;
        }
        o.push_back(i_name[i]);
    }
    if (i_name[i_name.length() - 1] != ' ') {
        o.push_back(i_name[i_name.length() - 1]);
    }

    m_name = o;
}

std::string Row::name() const {
    return m_name;
}

void Row::setTotalValue(float i_totalValue) {
    if (i_totalValue < (__FLT32_MIN__)) {
        m_totalValue = (__FLT32_MIN__);
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'total value' to value lower than MIN_FLOAT, clamping to MIN_FLOAT\n";}
        return;
    }
    if (i_totalValue > __FLT32_MAX__) {
        m_totalValue = __FLT32_MAX__;
        if (m_verbose) {std::cerr << "Warning: Attempted to set 'total value' to value higher than MAX_FLOAT, clamping to MAX_FLOAT\n";}
        return;
    }
    m_totalValue = i_totalValue;
}

float Row::totalValue() const {
    return m_totalValue;
}
