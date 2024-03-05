//
// Created by pablo on 3/5/24.
//

#include "ScaleRects.h"

QString ScaleRects::caption() const {
    return QString("Scale Rects");
}

QString ScaleRects::name() const {
    return QString("Scale Rects");
}

unsigned ScaleRects::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType ScaleRects::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                case 1:
                    return VariantData(QSize()).typeIn();
                default:
                    return RectsData().type();
            }
        case QtNodes::PortType::Out:
            return RectsData().type();
        default:
            return RectsData().type();
    }
}

void ScaleRects::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inRectsData = std::dynamic_pointer_cast<RectsData>(nodeData);
            break;
        case 1:
            m_inScaleFactor = std::dynamic_pointer_cast<VariantData>(nodeData);
            break;
        default:
            break;
    }
    const auto lockRects = m_inRectsData.lock();
    const auto lockScaleFactor = m_inScaleFactor.lock();
    if (lockRects && lockScaleFactor) {
        const auto rects = lockRects->rects();
        const auto scaleFactor = lockScaleFactor->variant().toSize();
        QList<QRect> scaledRects;
        for (const auto& rect: rects) {
            const QRect result(rect.x() * scaleFactor.width(), rect.y() * scaleFactor.height(),
                               rect.width() * scaleFactor.width(), rect.height() * scaleFactor.height());
            scaledRects.push_back(result);
        }
        m_outRectsData = std::make_shared<RectsData>(scaledRects);
    } else {
        m_outRectsData.reset();
    }
    emit dataUpdated(0);
}

std::shared_ptr<QtNodes::NodeData> ScaleRects::outData(const QtNodes::PortIndex port) {
    return m_outRectsData;
}

QWidget* ScaleRects::embeddedWidget() {
    return nullptr;
}
