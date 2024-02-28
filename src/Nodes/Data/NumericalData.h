//
// Created by pablo on 2/25/24.
//

#ifndef NUMERICALDATA_H
#define NUMERICALDATA_H

#include <QtNodes/NodeData>

class NumericalData final : public QtNodes::NodeData {
public:
    NumericalData() {
    }

    explicit NumericalData(const int number) : m_number(number) {
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"numerical", "Numerical"};
    }

    int number() const {
        return m_number;
    }

    void setNumber(const int number) {
        m_number = number;
    }

private:
    int m_number = 0;
};


#endif //NUMERICALDATA_H
