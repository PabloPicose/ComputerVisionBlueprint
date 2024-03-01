//
// Created by pablo on 3/1/24.
//

#ifndef DETECTMULTISCALEMODEL_H
#define DETECTMULTISCALEMODEL_H

#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/FileData.h"
#include "Nodes/Data/CascadeClassifierData.h"
#include "Nodes/Data/VariantData.h"
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/RectsData.h"

#include <QFutureWatcher>

namespace Ui {
    class DetectMultiScaleForm;
}

class DetectMultiScaleModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    DetectMultiScaleModel();

    ~DetectMultiScaleModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QString portCaption(QtNodes::PortType, QtNodes::PortIndex) const override;

private slots:
    void processFinished();

private:
    static QPair<QList<QRect>, quint64> processMultiScale(const QImage& image, cv::CascadeClassifier cc,
                                                          double scaleFactor, int minNeighbors, int flags = 0,
                                                          const QSize& minSize = QSize(),
                                                          const QSize& maxSize = QSize());

    void requestProcess();

    void updateInData();

    QImage getLatestImageToProcess() const;

private:
    QWidget* m_widget = nullptr;
    std::unique_ptr<Ui::DetectMultiScaleForm> m_ui;
    QFutureWatcher<QPair<QList<QRect>, quint64>> m_watcher;

    // in
    // 0
    QImage m_lastImageToProcess;
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    std::weak_ptr<CascadeClassifierData> m_inFileData;
    // 2
    double m_inScaleFactor = 1.1;
    std::weak_ptr<VariantData> m_inScaleFactorData;
    // 3
    int m_inMinNeighbors = 3;
    std::weak_ptr<VariantData> m_inMinNeighborsData;
    // 4
    QSize m_inMinSize = QSize(0,0);
    std::weak_ptr<VariantData> m_inMinSizeData;
    // 5
    QSize m_inMaxSize = QSize(0,0);
    std::weak_ptr<VariantData> m_inMaxSizeData;

    // out
    std::shared_ptr<RectsData> m_outImageData;
};


#endif //DETECTMULTISCALEMODEL_H
