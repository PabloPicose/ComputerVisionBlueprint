//
// Created by pablo on 3/1/24.
//

#ifndef CASCADECLASSIFIER_H
#define CASCADECLASSIFIER_H


#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/FileData.h"
#include "Nodes/Data/CascadeClassifierData.h"

namespace Ui {
    class CascadeClassifierForm;
}

class CascadeClassifier final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    CascadeClassifier();

    ~CascadeClassifier() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    std::unique_ptr<Ui::CascadeClassifierForm> m_ui;
    QWidget* m_widget = nullptr;

    std::weak_ptr<FileData> m_inFileData;

    std::shared_ptr<CascadeClassifierData> m_outFileData;
};


#endif //CASCADECLASSIFIER_H
