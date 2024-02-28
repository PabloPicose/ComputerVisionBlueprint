//
// Created by pablo on 2/25/24.
//

#ifndef SIZEDATA_H
#define SIZEDATA_H

#include <QtNodes/NodeData>
#include <QSize>

class SizeData final : public QtNodes::NodeData {
public:
    SizeData() = default;

    explicit SizeData(const QSize& size) : m_size(size) {
    }

    void setSize(const QSize& size) {
        m_size = size;
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"size", "Size"};
    }

    QSize size() const {
        return m_size;
    }

private:
    QSize m_size;
};

#endif //SIZEDATA_H
