//
// Created by pablo on 3/9/24.
//

#include "CutImageModel.h"
#include "Nodes/Data/VariantData.h"

QString CutImageModel::caption() const {
    return QString("Cut Image");
}

QString CutImageModel::name() const {
    return QString("Cut Image");
}

unsigned CutImageModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType CutImageModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return RectData().type();
                default:
                    return VariantData().type();
            }
        case QtNodes::PortType::Out:
            return ImageData().type();
        default:
            return VariantData().type();
    }
}

void CutImageModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            break;
        case 1:
            m_inRectData = std::dynamic_pointer_cast<RectData>(nodeData);
            break;
    }

    if (const auto lock = m_inImageData.lock()) {
        if (const auto lockRect = m_inRectData.lock()) {
            m_outImageData = std::make_shared<ImageData>(processImage(lock->image(), lockRect->rect()));
        } else {
            m_outImageData.reset();
        }
    } else {
        m_outImageData.reset();
    }
    emit dataUpdated(0);
}

std::shared_ptr<QtNodes::NodeData> CutImageModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* CutImageModel::embeddedWidget() {
    return nullptr;
}

QImage CutImageModel::processImage(const QImage& image, const QRect& rect) {
    if (rect.isNull()) {
        return QImage();
    }
    if (rect.isNull()) {
        return QImage();
    }
    return image.copy(rect);
}
