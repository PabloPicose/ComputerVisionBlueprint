//
// Created by pablo on 2/28/24.
//

#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QtNodes/NodeData>

#include <QImage>
#include <QPixmap>

class ImageData final : public QtNodes::NodeData {
public:
    ImageData() = default;

    explicit ImageData(QImage const &image) : m_image(image) {}

    explicit ImageData(QString const &fileName) : m_image(fileName) {}

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"image", "Image"};
    }

    bool isNull() const { return m_image.isNull(); }

    QImage image() const { return m_image; }

    QPixmap pixmap() const { return QPixmap::fromImage(m_image); }

private:
    QImage m_image;
};
#endif //IMAGEDATA_H
