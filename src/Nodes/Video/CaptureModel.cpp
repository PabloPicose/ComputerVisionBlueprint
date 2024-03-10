//
// Created by pablo on 3/9/24.
//

#include "CaptureModel.h"

#include <QPushButton>

QString CaptureModel::caption() const {
    return QString("Capture");
}

QString CaptureModel::name() const {
    return QString("Capture");
}

unsigned CaptureModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 1;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType CaptureModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}

void CaptureModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            break;
    }
}

std::shared_ptr<QtNodes::NodeData> CaptureModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* CaptureModel::embeddedWidget() {
    if (!m_button) {
        m_button = new QPushButton("Capture");
        connect(m_button, &QPushButton::clicked, this, &CaptureModel::captureClicked);
    }
    return m_button;
}

void CaptureModel::captureClicked() {
    if (const auto lock = m_inImageData.lock()) {
        m_outImageData = std::make_shared<ImageData>(lock->image());
    } else {
        m_outImageData.reset();
    }
    emit dataUpdated(0);
}
