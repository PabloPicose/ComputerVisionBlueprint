//
// Created by pablo on 2/26/24.
//

#ifndef GAUSSIANBLURMODEL_H
#define GAUSSIANBLURMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>
#include "Nodes/Data/ImageData.h"

namespace Ui {
    class GaussianBlurForm;
}


class GaussianBlurModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    GaussianBlurModel();

    ~GaussianBlurModel() override;

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
    static std::tuple<QImage, quint64> processImage(QImage pixmap, const QSize& size, double sigmaX, double sigmaY);

private:
    std::unique_ptr<Ui::GaussianBlurForm> m_ui;
    QWidget* m_widget = nullptr;

    std::shared_ptr<ImageData> m_inPixmapData;

    QImage m_inPixmap;
    QImage m_outPixmap;
    QFutureWatcher<std::tuple<QImage, quint64>> m_watcher;
};


#endif //GAUSSIANBLURMODEL_H
