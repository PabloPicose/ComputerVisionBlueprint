//
// Created by pablo on 2/25/24.
//

#include "CannyModel.h"
#include "Nodes/Data/DataInclude.h"
#include <opencv2/opencv.hpp>
#include "Nodes/Conversor/MatQt.h"
#include <QtConcurrent/QtConcurrent>
#include "ui_CannyForm.h"

CannyModel::CannyModel() {
    connect(&m_watcher, &QFutureWatcher<QPixmap>::finished, this, &CannyModel::processFinished);
}

CannyModel::~CannyModel() {
}

QString CannyModel::caption() const {
    return QString("Canny");
}

QString CannyModel::name() const {
    return QString("Canny");
}

unsigned CannyModel::nPorts(QtNodes::PortType const portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 5;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType CannyModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    if (portType == QtNodes::PortType::In) {
        switch (portIndex) {
            case 0:
                return ImageData().type();
            case 1:
                return VariantData(0.0).typeIn();
            case 2:
                return VariantData(0.0).typeIn();
            case 3:
                return VariantData(0).typeIn();
            case 4:
                return VariantData(false).typeIn();
            default:
                qCritical() << "Invalid port index";
                return ImageData().type();
                break;
        }
    }
    return ImageData().type();
}

void CannyModel::setInData(std::shared_ptr<QtNodes::NodeData> const nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            const auto lock = m_inImageData.lock();
            if (!lock) {
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            } else {
                m_lastImageToProcess = lock->image();
            }
        }
        break;
        case 1: {
            QSignalBlocker blocker(m_ui->sb_threshold);
            const auto decimalData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (decimalData && decimalData->metaType() == QMetaType::Double) {
                m_ui->sb_threshold->setEnabled(false);
                m_ui->sb_threshold->setValue(decimalData->variant().toDouble());
                m_lowThreshold = decimalData->variant().toDouble();
            } else {
                m_ui->sb_threshold->setEnabled(true);
            }
        }
        break;
        case 2: {
            QSignalBlocker blocker(m_ui->sb_threshold2);
            const auto decimalData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (decimalData && decimalData->metaType() == QMetaType::Double){
                m_ui->sb_threshold2->setEnabled(false);
                m_ui->sb_threshold2->setValue(decimalData->variant().toDouble());
                m_highThreshold = decimalData->variant().toDouble();
            } else {
                m_ui->sb_threshold2->setEnabled(true);
            }
        }
        break;
        case 3: {
            QSignalBlocker blocker(m_ui->sb_apertureSize);
            const auto numericalData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (numericalData && numericalData->metaType() == QMetaType::Int){
                m_ui->sb_apertureSize->setEnabled(false);
                m_ui->sb_apertureSize->setValue(numericalData->variant().toDouble());
                m_apertureSize = numericalData->variant().toInt();
            } else {
                m_ui->sb_apertureSize->setEnabled(true);
            }
        }
        break;
        case 4: {
            const auto booleanData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (booleanData && booleanData->metaType() == QMetaType::Bool){
                m_ui->cb_gradient->setEnabled(false);
                m_useL2Gradient = booleanData->variant().toBool();
                m_ui->cb_gradient->setChecked(m_useL2Gradient);
            }
            else {
                m_ui->cb_gradient->setEnabled(true);
            }
        }
        break;

        default:
            qCritical() << "Invalid port index";
            break;
    }

    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> CannyModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* CannyModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = new Ui::CannyForm();
        m_ui->setupUi(m_widget);
        // connect the QDoubleSpinBox sb_threshold, sb_threshold2
        connect(m_ui->sb_threshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_lowThreshold = value;
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->sb_threshold2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_highThreshold = value;
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->sb_apertureSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_apertureSize = value;
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->cb_gradient, &QCheckBox::checkStateChanged, this, [this](Qt::CheckState state) {
            m_useL2Gradient = state == Qt::Checked;
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
    }
    return m_widget;
}

QImage CannyModel::getImageToProcess() const {
    const auto imagePtr = m_inImageData.lock();
    if (!imagePtr) {
        return QImage();
    }
    return imagePtr->image();
}

void CannyModel::processFinished() {
    m_outImageData = std::make_shared<ImageData>(m_watcher.result());
    emit dataUpdated(0);
    requestProcess();
}

void CannyModel::requestProcess() {
    if (m_watcher.isRunning()) {
        m_watcher.cancel();
    }
    if (m_lastImageToProcess.isNull()) {
        return;
    }

    const auto future = QtConcurrent::run(processImage, m_lastImageToProcess, m_lowThreshold, m_highThreshold,
                                          m_apertureSize, m_useL2Gradient);
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

QImage CannyModel::processImage(const QImage& image, const double lowThreshold, const double highThreshold,
                                const int apertureSize,
                                const bool useL2Gradient) {
    const cv::Mat src = QImageToMat(image);
    cv::Mat dst;
    try {
        cv::Canny(src, dst, lowThreshold, highThreshold, apertureSize, useL2Gradient);
    } catch (cv::Exception& e) {
        qDebug() << e.what();
        return QImage();
    }
    return MatToQImage(dst);
}
