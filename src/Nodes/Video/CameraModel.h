//
// Created by pablo on 3/9/24.
//

#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H


#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/ImageData.h"

namespace Ui {
    class CameraForm;
}

class QLabel;
class QCamera;
class QComboBox;
class QVideoFrame;
class QMediaCaptureSession;
class QVideoSink;
class QCameraDevice;

class CameraModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    CameraModel();

    ~CameraModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    bool resizable() const override { return true; }


private slots:
    void onDeviceChanged(int index);

    void updateCameras();

    void setCamera(const QCameraDevice& device);

    void onFrameAvailable(const QVideoFrame& frame);

private:
    QWidget* m_widget = nullptr;
    QScopedPointer<Ui::CameraForm> m_ui;

    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QMediaCaptureSession> m_captureSession;
    QScopedPointer<QVideoSink> m_videoSink;

    // out
    std::shared_ptr<ImageData> m_outImageData;
};


#endif //CAMERAMODEL_H
