#include "MathOperationDataModel.hpp"


unsigned int MathOperationDataModel::nPorts(QtNodes::PortType portType) const
{
    unsigned int result;

    if (portType == QtNodes::PortType::In)
        result = 2;
    else
        result = 1;

    return result;
}

QtNodes::NodeDataType MathOperationDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex) const
{
    switch (portType) {
        case QtNodes::PortType::In: {
            const auto lock1 = _number1.lock();
            if (lock1) {
                return lock1->typeIn();
            } else {
                return VariantData().type();
            }
            const auto lock2 = _number2.lock();
            if (lock2) {
                return lock2->typeIn();
            } else {
                return VariantData().type();
            }
        }
        case QtNodes::PortType::Out: {
            if (_result) {
                return _result->type();
            } else {
                return VariantData().type();
            }
        }
        default:
            return VariantData().type();
    }
}

std::shared_ptr<QtNodes::NodeData> MathOperationDataModel::outData(QtNodes::PortIndex)
{
    return _result;
}

void MathOperationDataModel::setInData(std::shared_ptr<QtNodes::NodeData> const data, QtNodes::PortIndex portIndex)
{
    const auto numberData = std::dynamic_pointer_cast<VariantData>(data);

    if (!data) {
        Q_EMIT dataInvalidated(0);
    }

    if (portIndex == 0) {
        _number1 = numberData;
    } else {
        _number2 = numberData;
    }

    _result = compute();
    emit dataUpdated(0);
}
