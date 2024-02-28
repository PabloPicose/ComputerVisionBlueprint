//
// Created by pablo on 2/27/24.
//

#ifndef PIMODEL_H
#define PIMODEL_H


#include <QtNodes/NodeDelegateModel>

class DecimalData;

class PiModel final : public QtNodes::NodeDelegateModel{
    Q_OBJECT
public:
    PiModel();

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    std::shared_ptr<DecimalData> m_pi;
};



#endif //PIMODEL_H
