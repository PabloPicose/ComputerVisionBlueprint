//
// Created by pablo on 3/9/24.
//

#include "CameraModel.h"
#include "ui_CameraForm.h"

#include <QCamera>
#include <QCameraDevice>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMediaDevices>


CameraModel::CameraModel() {
    m_captureSession.reset(new QMediaCaptureSession);
    m_videoSink.reset(new QVideoSink);
    connect(m_videoSink.data(), &QVideoSink::videoFrameChanged, this, &CameraModel::onFrameAvailable);
    setCamera(QMediaDevices::defaultVideoInput());
}

CameraModel::~CameraModel() {
}

QString CameraModel::caption() const {
    return QString("Camera");
}

QString CameraModel::name() const {
    return QString("Camera");
}

unsigned CameraModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 0;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType CameraModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return ImageData().type();
        case QtNodes::PortType::Out:
            return ImageData().type();
        default:
            return ImageData().type();
    }
}

void CameraModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> CameraModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* CameraModel::embeddedWidget() {
    if (!m_widget) {
        m_ui.reset(new Ui::CameraForm);
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        // cb_devices QComboBox
        connect(m_ui->cb_devices, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &CameraModel::onDeviceChanged);
        updateCameras();
    }
    return m_widget;
}


void CameraModel::onDeviceChanged(const int index) {
    const QList<QCameraDevice> devices = QMediaDevices::videoInputs();
    if (index < devices.size()) {
        setCamera(devices[index]);
    }
}

void CameraModel::updateCameras() {
    if (m_ui) {
        m_ui->cb_devices->clear();
        const QList<QCameraDevice> devices = QMediaDevices::videoInputs();
        for (const QCameraDevice& device: devices) {
            m_ui->cb_devices->addItem(device.description());
        }
    }
}

void CameraModel::setCamera(const QCameraDevice& device) {
    m_camera.reset(new QCamera(device));
    m_captureSession->setCamera(m_camera.data());

    m_captureSession->setVideoSink(m_videoSink.data());

    m_camera->start();
}

void CameraModel::onFrameAvailable(const QVideoFrame& frame) {
    QVideoFrame copy(frame);
    if (copy.map(QVideoFrame::ReadOnly)) {
        const QImage image = copy.toImage();
        m_outImageData = std::make_shared<ImageData>(image);
        emit dataUpdated(0);
        // update ui label with image
        if (m_ui) {
            m_ui->cb_takingFrame->setChecked(true);
        }
    }
    else {
        m_outImageData.reset();
        emit dataUpdated(0);
        if (m_ui) {
            m_ui->cb_takingFrame->setChecked(false);
        }
    }
}
