//
// Created by pablo on 3/1/24.
//

#ifndef POINTDATA_H
#define POINTDATA_H


#include <QtNodes/NodeData>
#include <QPoint>

class PointData final : public QtNodes::NodeData {
public:
    PointData() {
    }
    explicit PointData(const QPoint& point) : m_point(point) {
    }

    QPoint point() const {
        return m_point;
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"point", "Point"};
    }
private:
    QPoint m_point;
};

#endif //POINTDATA_H
