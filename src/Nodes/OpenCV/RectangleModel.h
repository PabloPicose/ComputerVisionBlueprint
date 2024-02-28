//
// Created by pablo on 2/26/24.
//

#ifndef RECTANGLEMODEL_H
#define RECTANGLEMODEL_H

#include <QtNodes/NodeDelegateModel>

QT_BEGIN_NAMESPACE

namespace Ui {
    class RectangleForm;
}

QT_END_NAMESPACE

class RectangleModel : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    explicit RectangleModel();

    ~RectangleModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    Ui::RectangleForm* ui = nullptr;
};


#endif //RECTANGLEMODEL_H
