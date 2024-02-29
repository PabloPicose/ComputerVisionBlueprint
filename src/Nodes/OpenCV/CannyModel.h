//
// Created by pablo on 2/25/24.
//

#ifndef CANNYMODEL_H
#define CANNYMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"


namespace Ui {
    class CannyForm;
}

class CannyModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    CannyModel();

    ~CannyModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QImage getImageToProcess() const;

private:
    static QImage processImage(const QImage& image, double lowThreshold, double highThreshold,
                                int apertureSize, bool useL2Gradient);

private slots:
    void processFinished();

    void requestProcess();

private:
    QWidget* m_widget = nullptr;
    Ui::CannyForm* m_ui = nullptr;
    // in
    // 0
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    double m_lowThreshold = 0.0;
    // 2
    double m_highThreshold = 0.0;
    // 3
    int m_apertureSize = 3;
    // 4
    bool m_useL2Gradient = false;

    // out
    std::shared_ptr<ImageData> m_outImageData;
    QImage m_lastImageToProcess;

    QFutureWatcher<QImage> m_watcher;
};


#endif //CANNYMODEL_H
