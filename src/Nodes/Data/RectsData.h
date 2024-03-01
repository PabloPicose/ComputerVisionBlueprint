//
// Created by pablo on 3/1/24.
//

#ifndef RECTS_H
#define RECTS_H

#include <QtNodes/NodeData>
#include <QRect>
#include <QList>

typedef QList<QRect> Rects;

class RectsData final : public QtNodes::NodeData {
public:
    RectsData() {
    }
    explicit RectsData(const QList<QRect>& rects) : m_rects(rects) {
    }

    QList<QRect> rects() {
        return m_rects;
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"rects", "Rects"};
    }
private:
    QList<QRect> m_rects;
};

#endif //RECTS_H
