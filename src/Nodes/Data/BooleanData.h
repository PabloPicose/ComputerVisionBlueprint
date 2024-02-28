//
// Created by pablo on 2/25/24.
//

#ifndef BOOLEANDATA_H
#define BOOLEANDATA_H

#include <QtNodes/NodeData>

class BooleanData final : public QtNodes::NodeData {
public:
    BooleanData() {
    }

    explicit BooleanData(const bool boolean) : m_boolean(boolean) {
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"boolean", "Boolean"};
    }

    bool boolean() const {
        return m_boolean;
    }

    void setBoolean(const bool boolean) {
        m_boolean = boolean;
    }
private:
    bool m_boolean = false;
};

#endif //BOOLEANDATA_H
