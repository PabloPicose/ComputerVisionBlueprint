//
// Created by pablo on 3/1/24.
//

#ifndef DRAWRECTSMODEL_H
#define DRAWRECTSMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/RectsData.h"
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class DrawRectsForm;
}

class DrawRectsModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    DrawRectsModel();

    ~DrawRectsModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    static QPair<QImage, quint64> processImage(QImage image, const Rects& rects, const QColor& color, int thickness);

    void updateFromInputPort();

    QImage getPixmapToProcess() const;

private slots:
    void processFinished();

    void requestProcess();
private:
    QWidget* m_widget = nullptr;
    QScopedPointer<Ui::DrawRectsForm> m_ui;
    QFutureWatcher<QPair<QImage, quint64>> m_watcher;

    // in
    // 0
    QImage m_lastPixmapToProcess;
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    std::weak_ptr<RectsData> m_inRectsData;
    // 2
    QColor m_color;
    std::weak_ptr<VariantData> m_inColor;
    // 3
    int m_thickness;
    std::weak_ptr<VariantData> m_inThickness;

    // out
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //DRAWRECTSMODEL_H
