//
// Created by pablo on 3/9/24.
//

#ifndef CAPTUREMODEL_H
#define CAPTUREMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/ImageData.h"

class QPushButton;

class CaptureModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private slots:
    void captureClicked();

private:
    QPushButton* m_button = nullptr;
    // in
    // 0
    std::weak_ptr<ImageData> m_inImageData;
    // out
    // 0
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //CAPTUREMODEL_H
