#include "NumberDisplayDataModel.hpp"

#include <QtWidgets/QLabel>

NumberDisplayDataModel::NumberDisplayDataModel()
    : _label{nullptr}
{}

unsigned int NumberDisplayDataModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType) {
    case PortType::In:
        result = 1;
        break;

    case PortType::Out:
        result = 0;

    default:
        break;
    }

    return result;
}

NodeDataType NumberDisplayDataModel::dataType(PortType, PortIndex) const
{
    const auto lockData = _numberData.lock();
    if (lockData) {
        return lockData->typeIn();
    } else {
        return VariantData().typeIn();
    }
}

std::shared_ptr<NodeData> NumberDisplayDataModel::outData(PortIndex)
{
    std::shared_ptr<NodeData> ptr;
    return ptr;
}

void NumberDisplayDataModel::setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
{
    _numberData = std::dynamic_pointer_cast<VariantData>(data);
    const auto lockData = _numberData.lock();

    if (!_label)
        return;

    if (lockData) {
        _label->setText(lockData->toString());
    } else {
        _label->clear();
    }

    _label->adjustSize();
}

QWidget *NumberDisplayDataModel::embeddedWidget()
{
    if (!_label) {
        _label = new QLabel();
        _label->setMargin(3);
    }

    return _label;
}
