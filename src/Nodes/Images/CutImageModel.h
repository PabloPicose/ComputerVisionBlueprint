//
// Created by pablo on 3/9/24.
//

#ifndef CUTIMAGEMODEL_H
#define CUTIMAGEMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/RectData.h"
#include "Nodes/Data/ImageData.h"


class CutImageModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    static QImage processImage(const QImage& image, const QRect& rect);

private:
    // in
    // 0
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    std::weak_ptr<RectData> m_inRectData;

    // out
    // 0
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //CUTIMAGEMODEL_H
