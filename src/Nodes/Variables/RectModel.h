//
// Created by pablo on 3/9/24.
//

#ifndef RECTMODEL_H
#define RECTMODEL_H


#include <opencv2/core/types.hpp>
#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/RectData.h"
#include "Nodes/Data/PointData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class RectForm;
}

class RectModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    RectModel();

    ~RectModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override;

    QString portCaption(QtNodes::PortType, QtNodes::PortIndex) const override;

private slots:
    void updateRectFromUI();

    void updateCenter();

private:
    QWidget* m_widget = nullptr;
    std::unique_ptr<Ui::RectForm> m_ui;
    // in
    // 0
    std::weak_ptr<RectData> m_inRectData;
    //out
    // 0
    QRect m_outRect;
    std::shared_ptr<RectData> m_outRectsData;
    // 1
    std::shared_ptr<PointData> m_outCenterData;

};


#endif //RECTMODEL_H
