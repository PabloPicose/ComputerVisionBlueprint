//
// Created by pablo on 2/27/24.
//

#ifndef HOUGHLINESP_H
#define HOUGHLINESP_H


#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>
#include <opencv2/opencv.hpp>
#include "Nodes/Data/LinesSegmentData.h"
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"

namespace Ui {
    class HoughLinesPForm;
}


class HoughLinesP final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    HoughLinesP();

    ~HoughLinesP() override;

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
    static std::tuple<LinesSegmentData, quint64> processImage(QImage image, double rho, double theta,
                                                              int threshold,
                                                              double minLineLength, double maxLineGap);

    QImage getPixmapToProcess();

private:
    std::unique_ptr<Ui::HoughLinesPForm> m_ui;
    QWidget* m_widget = nullptr;


    QImage m_lastPixmapToProcess;
    QFutureWatcher<std::tuple<LinesSegmentData, quint64>> m_watcher;
    std::shared_ptr<LinesSegmentData> m_outLinesData;

    std::weak_ptr<ImageData> m_inPixmapData;
    double m_rho = 1;
    double m_theta = CV_PI / 180;
    int m_threshold = 50;
    double m_minLineLength = 50;
    double m_maxLineGap = 10;
};


#endif //HOUGHLINESP_H
