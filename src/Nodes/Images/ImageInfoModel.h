//
// Created by pablo on 2/29/24.
//

#ifndef IMAGEINFOMODEL_H
#define IMAGEINFOMODEL_H

#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"
#include "Nodes/Data/ImageFormatData.h"

namespace Ui {
    class ImageInfoForm;
}

class ImageInfoModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    ImageInfoModel();

    ~ImageInfoModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    void invalidateOutData();

    void updateData();

    static QString formatToString(QImage::Format format);

private:
    std::unique_ptr<Ui::ImageInfoForm> m_ui;
    QWidget* m_widget = nullptr;

    // in
    std::weak_ptr<ImageData> m_inImageData;
    // out
    // 0
    std::shared_ptr<ImageData> m_outImageData;
    // 1
    std::shared_ptr<ImageFormatData> m_outFormat;
    // 2
    std::shared_ptr<VariantData> m_outSize;
};



#endif //IMAGEINFOMODEL_H
