//
// Created by pablo on 3/5/24.
//

#ifndef SCALERECTS_H
#define SCALERECTS_H

#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/VariantData.h"
#include "Nodes/Data/RectsData.h"

class ScaleRects final : public QtNodes::NodeDelegateModel {
    Q_OBJECT
public:
    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;
private:
    // in
    // 0
    std::weak_ptr<RectsData> m_inRectsData;
    // 1
    std::weak_ptr<VariantData> m_inScaleFactor;
    // out
    // 0
    std::shared_ptr<RectsData> m_outRectsData;
};



#endif //SCALERECTS_H
