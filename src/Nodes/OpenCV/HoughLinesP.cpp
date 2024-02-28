//
// Created by pablo on 2/27/24.
//

#include "HoughLinesP.h"

#include <opencv2/opencv.hpp>

#include "ui_HoughLinesPForm.h"

#include "Nodes/Data/DataInclude.h"

#include <QtConcurrent/QtConcurrent>
#include "Nodes/Conversor/MatQt.h"
#include <QElapsedTimer>

HoughLinesP::HoughLinesP() {
    /*cv::CascadeClassifier faceCascade;
    cv::Mat image;
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(image, faces, 1.2, 5, 0, cv::Size(30, 30));

    cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0);
    cv::dilate(image, image, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
    cv::HoughLinesP(image, image, 1, CV_PI / 180, 50, 50, 10);
    */
    connect(&m_watcher, &QFutureWatcher<std::tuple<QPixmap, quint64>>::finished, this, &HoughLinesP::processFinished);
}

HoughLinesP::~HoughLinesP() {
}

QString HoughLinesP::caption() const {
    return QString("Hough Lines Probabilistic");
}

QString HoughLinesP::name() const {
    return QString("HoughLinesP");
}

unsigned HoughLinesP::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 6;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType HoughLinesP::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    if (portType == QtNodes::PortType::In) {
        switch (portIndex) {
            case 0:
                return ImageData().type();
            case 1:
                return DecimalData().type();
            case 2:
                return DecimalData().type();
            case 3:
                return NumericalData().type();
            case 4:
                return DecimalData().type();
            case 5:
                return DecimalData().type();
            default:
                qCritical() << "Invalid port index";
                return ImageData().type();
        }
    }
    return LinesSegmentData().type();
}

void HoughLinesP::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inPixmapData = std::dynamic_pointer_cast<ImageData>(nodeData);
            const auto lockData = m_inPixmapData.lock();
            if (lockData) {
                m_lastPixmapToProcess = lockData->image();
            } else {
                m_outLinesData.reset();
                emit dataUpdated(0);
            }
            break;
        }
        case 1: {
            const auto decimalData = std::dynamic_pointer_cast<DecimalData>(nodeData);
            if (decimalData) {
                const double decimal = decimalData->number();
                QSignalBlocker blocker(m_ui->sb_rho);
                m_ui->sb_rho->setValue(decimal);
                m_rho = decimal;
                m_ui->sb_rho->setEnabled(false);
            } else {
                m_ui->sb_rho->setEnabled(true);
            }
            break;
        }
        case 2: {
            const auto decimalData = std::dynamic_pointer_cast<DecimalData>(nodeData);
            if (decimalData) {
                const double decimal = decimalData->number();
                QSignalBlocker blocker(m_ui->sb_theta);
                m_ui->sb_theta->setValue(decimal);
                m_theta = decimal;
                m_ui->sb_theta->setEnabled(false);
            } else {
                m_ui->sb_theta->setEnabled(true);
            }
            break;
        }
        case 3: {
            const auto numericalData = std::dynamic_pointer_cast<NumericalData>(nodeData);
            if (numericalData) {
                const int number = numericalData->number();
                QSignalBlocker blocker(m_ui->sb_threshold);
                m_ui->sb_threshold->setValue(number);
                m_threshold = number;
                m_ui->sb_threshold->setEnabled(false);
            } else {
                m_ui->sb_threshold->setEnabled(true);
            }
            break;
        }
        case 4: {
            const auto decimalData = std::dynamic_pointer_cast<DecimalData>(nodeData);
            if (decimalData) {
                const double decimal = decimalData->number();
                QSignalBlocker blocker(m_ui->sb_minLineLength);
                m_ui->sb_minLineLength->setValue(decimal);
                m_minLineLength = decimal;
                m_ui->sb_minLineLength->setEnabled(false);
            } else {
                m_ui->sb_minLineLength->setEnabled(true);
            }
            break;
        }
        case 5: {
            const auto decimalData = std::dynamic_pointer_cast<DecimalData>(nodeData);
            if (decimalData) {
                const double decimal = decimalData->number();
                QSignalBlocker blocker(m_ui->sb_maxLineGap);
                m_ui->sb_maxLineGap->setValue(decimal);
                m_maxLineGap = decimal;
                m_ui->sb_maxLineGap->setEnabled(false);
            } else {
                m_ui->sb_maxLineGap->setEnabled(true);
            }
            break;
        }
        default:
            break;
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> HoughLinesP::outData(const QtNodes::PortIndex port) {
    return m_outLinesData;
}

QWidget* HoughLinesP::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::HoughLinesPForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        connect(m_ui->sb_rho, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_rho = value;
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_theta, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_theta = value;
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_threshold, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_threshold = value;
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_minLineLength, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                [this](double value) {
                    m_minLineLength = value;
                    m_lastPixmapToProcess = getPixmapToProcess();
                    requestProcess();
                });
        connect(m_ui->sb_maxLineGap, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_maxLineGap = value;
            requestProcess();
        });
    }
    return m_widget;
}

void HoughLinesP::processFinished() {
    const auto tuple = m_watcher.result();
    if (m_inPixmapData.expired()) {
        m_outLinesData.reset();
    } else {
        m_outLinesData = std::make_shared<LinesSegmentData>(std::get<0>(tuple));
    }
    const quint64 time = std::get<1>(tuple);
    m_ui->sb_time->setValue(time);
    emit dataUpdated(0);
    requestProcess();
}

void HoughLinesP::requestProcess() {
    if (m_watcher.isRunning()) {
        return;
    }
    if (m_lastPixmapToProcess.isNull()) {
        return;
    }
    const auto future = QtConcurrent::run(processImage, m_lastPixmapToProcess, m_ui->sb_rho->value(),
                                          m_ui->sb_theta->value(),
                                          m_ui->sb_threshold->value(), m_ui->sb_minLineLength->value(),
                                          m_ui->sb_maxLineGap->value());
    m_lastPixmapToProcess = QImage();
    m_watcher.setFuture(future);
}

std::tuple<LinesSegmentData, quint64> HoughLinesP::processImage(const QImage image, const double rho,
                                                                const double theta,
                                                                const int threshold,
                                                                const double minLineLength, const double maxLineGap) {
    QElapsedTimer timer;
    timer.start();
    const cv::Mat src = QImageToMat(image);
    std::vector<cv::Vec4i> dst;
    try {
        cv::HoughLinesP(src, dst, rho, theta, threshold, minLineLength, maxLineGap);
    } catch (cv::Exception& e) {
        qDebug() << e.what();
        return std::make_tuple(LinesSegmentData(), timer.elapsed());
    }
    return std::make_tuple(LinesSegmentData(dst), timer.elapsed());
}

QImage HoughLinesP::getPixmapToProcess() {
    const auto lock = m_inPixmapData.lock();
    if (!lock) {
        return QImage();
    }
    return lock->image();
}
