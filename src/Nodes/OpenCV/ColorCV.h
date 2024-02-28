//
// Created by pablo on 2/24/24.
//

#ifndef COLORCV_H
#define COLORCV_H

#include <QtNodes/NodeDelegateModel>
#include <QComboBox>
#include <opencv2/opencv.hpp>

#include "Nodes/Data/ImageData.h"

class ColorCV final : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    ColorCV();

    ~ColorCV() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    std::weak_ptr<ImageData> m_inImageData;
    std::shared_ptr<ImageData> m_outImageData;
    QComboBox* m_comboBox = nullptr;
};



#endif //COLORCV_H
