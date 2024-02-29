#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtWidgets/QLabel>
#include <iostream>

#include "Nodes/Data/VariantData.h"


/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class MathOperationDataModel : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    ~MathOperationDataModel() override = default;

public:
    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) override;

    QWidget *embeddedWidget() override { return nullptr; }

protected:
    virtual std::shared_ptr<VariantData> compute() const = 0;

protected:
    std::weak_ptr<VariantData> _number1;
    std::weak_ptr<VariantData> _number2;
private:
    std::shared_ptr<VariantData> _result;
};
