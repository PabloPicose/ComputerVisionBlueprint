//
// Created by pablo on 2/29/24.
//

#ifndef IMAGEFORMATDATA_H
#define IMAGEFORMATDATA_H

#include <QtNodes/NodeData>
#include <QImage>
#include <QMetaType>
#include <QMetaEnum>

class ImageFormatData final : public QtNodes::NodeData {
public:
    ImageFormatData() = default;

    explicit ImageFormatData(const QImage::Format format) : m_format(format) {
    }

    explicit ImageFormatData(const QImage& image) : m_format(image.format()) {
    }

    QImage::Format format() const { return m_format; }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"imageFormat", "Image Format"};
    }

    static QString formatToString(const QImage::Format format) {
        const QMetaObject& metaObject = QImage::staticMetaObject;
        const int index = metaObject.indexOfEnumerator("Format");
        const QMetaEnum metaEnum = metaObject.enumerator(index);
        return metaEnum.valueToKey(format);
    }

private:
    QImage::Format m_format;
};
#endif //IMAGEFORMATDATA_H
