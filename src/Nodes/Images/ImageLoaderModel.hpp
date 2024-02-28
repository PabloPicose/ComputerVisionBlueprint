#pragma once

#include <iostream>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "Nodes/Data/ImageData.h"

class ImageLoaderModel final : public QtNodes::NodeDelegateModel
{
    Q_OBJECT

public:
    ImageLoaderModel();

    ~ImageLoaderModel() override = default;

public:
    QString caption() const override { return QString("Image Source"); }

    QString name() const override { return QString("ImageLoaderModel"); }

public:
    virtual QString modelName() const { return QString("Source Image"); }

    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex const portIndex) override {}

    QWidget *embeddedWidget() override { return _label; }

    bool resizable() const override { return true; }

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    QLabel *_label;

    std::shared_ptr<ImageData> m_outImageData;
};
