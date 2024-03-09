//
// Created by pablo on 3/1/24.
//

#ifndef RECTVARMODEL_H
#define RECTVARMODEL_H


#include <opencv2/core/types.hpp>
#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/RectsData.h"
#include "Nodes/Data/RectData.h"
#include "Nodes/Data/PointData.h"
#include "Nodes/Data/VariantData.h"


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

    //QJsonObject save() const override;

    //void load(QJsonObject const& jsonObj) override;

    QtNodes::ConnectionPolicy portConnectionPolicy(QtNodes::PortType, QtNodes::PortIndex) const override;


private slots:
    void updateRect();

    void inputConnectionDeleted(QtNodes::ConnectionId const&) override;

    void inputConnectionCreated(QtNodes::ConnectionId const&) override;

private:
    QWidget* m_widget = nullptr;

    // in
    QMap<int, std::weak_ptr<RectsData>> m_inVRectsDataMap;
    QMap<int, std::weak_ptr<RectData>> m_inRectDataMap;
    //std::weak_ptr<RectsData> m_inRectsData;

    // out
    //0
    std::shared_ptr<RectsData> m_outRectsData;
    QList<std::shared_ptr<RectData>> m_outRectsDataList;
};


#endif //RECTVARMODEL_H
