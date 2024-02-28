#pragma once

#include <QtGui/QPixmap>

#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph
class PixmapData final : public NodeData {
public:
    PixmapData() {
    }

    explicit PixmapData(QPixmap const& pixmap)
        : _pixmap(pixmap) {
    }

    NodeDataType type() const override {
        //       id      name
        return {"pixmap", "Pixmap"};
    }

    void setPixmap(QPixmap const& pixmap) {
        _pixmap = pixmap;
    }

    bool isNull() const { return _pixmap.isNull(); }

    QPixmap pixmap() const { return _pixmap; }

private:
    QPixmap _pixmap;
};
