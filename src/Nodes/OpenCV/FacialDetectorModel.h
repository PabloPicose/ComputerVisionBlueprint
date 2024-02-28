//
// Created by pablo on 2/26/24.
//

#ifndef FACIALDETECTORMODEL_H
#define FACIALDETECTORMODEL_H

#include <QtNodes/NodeDelegateModel>

class FacialDetectorModel final : public QtNodes::NodeDelegateModel{
    Q_OBJECT
public:
    FacialDetectorModel();

    ~FacialDetectorModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;
};



#endif //FACIALDETECTORMODEL_H
