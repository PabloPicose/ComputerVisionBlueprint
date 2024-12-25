#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/FileData.h"

class ImageFromFile final : public QtNodes::NodeDelegateModel
{
    Q_OBJECT
public:
    ImageFromFile();
    ~ImageFromFile() override = default;

    QString caption() const override { return QString("Image from file"); }
    QString name() const override { return QString("Image from file"); }

    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex const portIndex) override;

    QWidget* embeddedWidget() override;

private:
    void loadImage();

private:
    std::weak_ptr<FileData> m_inFileData;
    std::shared_ptr<ImageData> m_outImageData;
};