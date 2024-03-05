//
// Created by pablo on 3/5/24.
//

#ifndef SCALEIMAGEMODEL_H
#define SCALEIMAGEMODEL_H


#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class ScaleImageForm;
}

class ScaleImageModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    ScaleImageModel();

    ~ScaleImageModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;
private:
    static QPair<QImage, quint64> processImage(const QImage& image, const QSize& scaleFactor);

    void requestProcess();
private:
    QWidget* m_widget = nullptr;
    QScopedPointer<Ui::ScaleImageForm> m_ui;

    // in
    // 0
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    std::weak_ptr<VariantData> m_inScaleFactor;
    // out
    // 0
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //SCALEIMAGEMODEL_H
