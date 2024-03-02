//
// Created by pablo on 3/1/24.
//

#include "RectVarModel.h"
#include "ui_RectVarForm.h"

RectVarModel::RectVarModel() {
}

RectVarModel::~RectVarModel() {
}

QString RectVarModel::caption() const {
    return "Rect";
}

QString RectVarModel::name() const {
    return "Rect";
}

unsigned RectVarModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 1;
        case QtNodes::PortType::Out:
            return 5;
        default:
            return 0;
    }
}

QtNodes::NodeDataType RectVarModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                default:
                    return VariantData().type();
            }
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                case 1:
                    return PointData().type();
                case 2:
                    return PointData().type();
                case 3:
                    return VariantData(0).type();
                case 4:
                    return VariantData(0).type();
                default:
                    return VariantData().type();
            }
        default:
            return VariantData().type();
    }
}

void RectVarModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> RectVarModel::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outRectsData;
        case 1:
            return m_outCenterPointData;
        case 2:
            return m_outTopLeftPointData;
        case 3:
            return m_outWidth;
        case 4:
            return m_outHeight;
        default:
            return nullptr;
    }
}

QWidget* RectVarModel::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::RectVarForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
    }
    return m_widget;
}

QString RectVarModel::portCaption(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    switch (port) {
        case QtNodes::PortType::In:
            switch (port_index) {
                case 0:
                    return "Rects";
                default:
                    return "Unknown";
            }
        case QtNodes::PortType::Out:
            switch (port_index) {
                case 0:
                    return "Rects";
                case 1:
                    return "Center Point";
                case 2:
                    return "Top Left Point";
                case 3:
                    return "Width";
                case 4:
                    return "Height";
                default:
                    return "Unknown";
            }
        default:
            return NodeDelegateModel::portCaption(port, port_index);
    }
}
