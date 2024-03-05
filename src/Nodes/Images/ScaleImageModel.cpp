//
// Created by pablo on 3/5/24.
//

#include "ScaleImageModel.h"
#include "ui_ScaleImageForm.h"
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

ScaleImageModel::ScaleImageModel() {
}

ScaleImageModel::~ScaleImageModel() {
}

QString ScaleImageModel::caption() const {
    return QString("Scale Image");
}

QString ScaleImageModel::name() const {
    return QString("ScaleImage");
}

unsigned ScaleImageModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType ScaleImageModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return VariantData(QSize()).typeIn();
                default:
                    return ImageData().type();
            }
        case QtNodes::PortType::Out:
            return ImageData().type();
        default:
            return ImageData().type();
    }
}

void ScaleImageModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            break;
        case 1:
            m_inScaleFactor = std::dynamic_pointer_cast<VariantData>(nodeData);
            break;
        default:
            break;
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> ScaleImageModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* ScaleImageModel::embeddedWidget() {
    if (!m_widget) {
        m_ui.reset(new Ui::ScaleImageForm);
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
    }
    return m_widget;
}

QPair<QImage, quint64> ScaleImageModel::processImage(const QImage& image, const QSize& scaleFactor) {
    QElapsedTimer timer;
    timer.start();
    const auto scaledImage = image.scaled(scaleFactor, Qt::KeepAspectRatio);
    return {scaledImage, static_cast<quint64>(timer.elapsed())};
}

void ScaleImageModel::requestProcess() {
    const auto lockImage = m_inImageData.lock();
    const auto lockScaleFactor = m_inScaleFactor.lock();
    if (lockImage && lockScaleFactor) {
        const auto image = lockImage->image();
        const auto scaleFactor = lockScaleFactor->variant().toSize();
        if (image.isNull() || scaleFactor.isEmpty()) {
            m_outImageData.reset();
            return;
        }
        const auto [fst, snd] = processImage(image, scaleFactor);
        m_outImageData = std::make_shared<ImageData>(fst);
        if (m_ui) {
            m_ui->sb_time->setValue(snd);
        }
    } else {
        m_outImageData.reset();
    }
    emit dataUpdated(0);
}
