//
// Created by pablo on 2/26/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RectangleModel.h" resolved

#include "RectangleModel.h"
#include "ui_RectangleForm.h"


RectangleModel::RectangleModel() {
}

RectangleModel::~RectangleModel() {
}

QString RectangleModel::caption() const {
    return QStringLiteral("Rectangle");
}

QString RectangleModel::name() const {
    return QStringLiteral("Rectangle");
}

unsigned RectangleModel::nPorts(QtNodes::PortType portType) const {
}

QtNodes::NodeDataType RectangleModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
}

void RectangleModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> RectangleModel::outData(const QtNodes::PortIndex port) {
}

QWidget* RectangleModel::embeddedWidget() {
}
