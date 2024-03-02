//
// Created by pablo on 3/1/24.
//

#ifndef RECTVARMODEL_H
#define RECTVARMODEL_H



#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/RectsData.h"
#include "Nodes/Data/PointData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class RectVarForm;
}

class RectVarModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    RectVarModel();

    ~RectVarModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QString portCaption(QtNodes::PortType, QtNodes::PortIndex) const override;

    bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override { return true; }
private:
    QWidget* m_widget = nullptr;
    std::unique_ptr<Ui::RectVarForm> m_ui;

    std::weak_ptr<RectsData> m_inRectsData;

    // out
    //0
    std::shared_ptr<RectsData> m_outRectsData;
    //1
    std::shared_ptr<PointData> m_outCenterPointData;
    //2
    std::shared_ptr<PointData> m_outTopLeftPointData;
    //3
    std::shared_ptr<VariantData> m_outWidth;
    //4
    std::shared_ptr<VariantData> m_outHeight;

};



#endif //RECTVARMODEL_H
