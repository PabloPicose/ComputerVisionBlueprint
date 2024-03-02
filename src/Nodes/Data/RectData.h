//
// Created by pablo on 3/1/24.
//

#ifndef RECTDATA_H
#define RECTDATA_H

#include <QtNodes/NodeData>
#include <QRect>

class RectData final : public QtNodes::NodeData {
public:
    RectData() {
    }
    explicit RectData(const QRect& rect) : m_rect(rect) {
    }

    QRect rect() const {
        return m_rect;
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"rect", "Rect"};
    }
private:
    QRect m_rect;
};

#endif //RECTDATA_H
