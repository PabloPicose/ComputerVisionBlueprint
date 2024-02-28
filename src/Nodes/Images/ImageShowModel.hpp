#pragma once

#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>


#include "Nodes/Data/PixmapData.hpp"

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class ImageShowModel final : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    ImageShowModel();

    ~ImageShowModel() override = default;

public:
    QString caption() const override { return QString("Image Display"); }

    QString name() const override { return QString("ImageShowModel"); }

public:
    virtual QString modelName() const { return QString("Resulting Image"); }

    unsigned int nPorts(QtNodes::PortType portType) const override;

    NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> nodeData, QtNodes::PortIndex port) override;

    QWidget *embeddedWidget() override { return _label; }

    bool resizable() const override { return true; }

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *_label;

    std::weak_ptr<PixmapData> m_outData;
};
