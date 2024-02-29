//
// Created by pablo on 2/27/24.
//

#include "PiModel.h"
#include <opencv2/opencv.hpp>

PiModel::PiModel() {
    m_pi = std::make_shared<VariantData>(CV_PI);
}

QString PiModel::caption() const {
    return QStringLiteral("Pi");
}

QString PiModel::name() const {
    return QStringLiteral("Pi");
}

unsigned PiModel::nPorts(QtNodes::PortType portType) const {
    if (portType == QtNodes::PortType::In) {
        return 0;
    } else {
        return 1;
    }
}

QtNodes::NodeDataType PiModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    if (portType == QtNodes::PortType::Out) {
        return VariantData(0.0).type();
    } else {
        return {};
    }
}

void PiModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> PiModel::outData(const QtNodes::PortIndex port) {
    return m_pi;
}

QWidget* PiModel::embeddedWidget() {
    return nullptr;
}
