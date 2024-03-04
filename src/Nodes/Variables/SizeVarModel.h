//
// Created by pablo on 2/29/24.
//

#ifndef SIZEVARMODEL_H
#define SIZEVARMODEL_H

#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class SizeVarForm;
}

class SizeVarModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    SizeVarModel();

    ~SizeVarModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QJsonObject save() const override;

    void load(QJsonObject const& jsonObj) override;

private:
    void setOutSize(const QSize& size);

private:
    std::unique_ptr<Ui::SizeVarForm> m_ui;
    QWidget* m_widget = nullptr;

    // in
    std::weak_ptr<VariantData> m_inSizeData;
    // out
    // 0
    QSize m_outSize;
    std::shared_ptr<VariantData> m_outSizeData;
    // 1
    std::shared_ptr<VariantData> m_outWidthData;
    // 2
    std::shared_ptr<VariantData> m_outHeightData;
};



#endif //SIZEVARMODEL_H
