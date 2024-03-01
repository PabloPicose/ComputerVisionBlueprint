//
// Created by pablo on 2/29/24.
//

#ifndef DRAWLINESMODEL_H
#define DRAWLINESMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/LinesSegmentData.h"
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class DrawLinesForm;
}

class DrawLinesModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    DrawLinesModel();

    ~DrawLinesModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private slots:
    void processFinished();

    void requestProcess();

private:
    static QPair<QImage, quint64> processImage(QImage image, const LinesSegment& linesSegment,
                                               const QColor& color,
                                               int thickness);

    void updateFromInputPort();

private:
    QWidget* m_widget = nullptr;
    QScopedPointer<Ui::DrawLinesForm> m_ui;
    QFutureWatcher<QPair<QImage, quint64>> m_watcher;

    // in
    // 0
    QImage m_lastPixmapToProcess;
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    std::weak_ptr<LinesSegmentData> m_inLinesSegmentData;
    // 2
    QColor m_lastColor;
    std::weak_ptr<VariantData> m_inColor;
    // 3
    int m_lastThickness = 1;
    std::weak_ptr<VariantData> m_inThickness;

    // out
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //DRAWLINESMODEL_H
