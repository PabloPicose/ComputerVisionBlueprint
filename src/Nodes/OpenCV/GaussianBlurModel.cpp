//
// Created by pablo on 2/26/24.
//
#include <QtConcurrent/QtConcurrent>
#include <QElapsedTimer>

#include "GaussianBlurModel.h"
#include "Nodes/Data/DataInclude.h"
#include "ui_GaussianBlurForm.h"

#include "Nodes/Conversor/MatQt.h"

#include <opencv2/opencv.hpp>

GaussianBlurModel::GaussianBlurModel() {
    connect(&m_watcher, &QFutureWatcher<std::tuple<QPixmap, quint64>>::finished, this,
            &GaussianBlurModel::processFinished);
}

GaussianBlurModel::~GaussianBlurModel() {
}

QString GaussianBlurModel::caption() const {
    return "Gaussian Blur";
}

QString GaussianBlurModel::name() const {
    return "GaussianBlur";
}

unsigned GaussianBlurModel::nPorts(QtNodes::PortType portType) const {
    return 1;
}

QtNodes::NodeDataType GaussianBlurModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}

void GaussianBlurModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            const auto lock = m_inImageData.lock();
            if (lock) {
                m_lastImageToProcess = getImageToProcess();
            } else {
                m_outImageData.reset();
                m_lastImageToProcess = QImage();
                emit dataUpdated(0);
            }
            break;
        }
        default:
            break;
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> GaussianBlurModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* GaussianBlurModel::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::GaussianBlurForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        // width and height should be odd and positive
        connect(m_ui->sb_size_w, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            if (value % 2 == 0) {
                m_ui->sb_size_w->setValue(value + 1);
                return;
            }
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->sb_size_h, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            if (value % 2 == 0) {
                m_ui->sb_size_h->setValue(value + 1);
                return;
            }
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->sigmaXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this]() {
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        connect(m_ui->sigmaYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this]() {
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
    }
    return m_widget;
}

void GaussianBlurModel::processFinished() {
    const auto tuple = m_watcher.result();
    if (m_inImageData.expired()) {
        m_outImageData.reset();
    } else {
        m_outImageData = std::make_shared<ImageData>(std::get<0>(tuple));
    }
    const quint64 time = std::get<1>(tuple);
    m_ui->sb_time->setValue(time);
    Q_EMIT dataUpdated(0);
    requestProcess();
}

void GaussianBlurModel::requestProcess() {
    if (m_watcher.isRunning()) {
        return;
    }
    if (m_lastImageToProcess.isNull()) {
        return;
    }
    const auto future = QtConcurrent::run(processImage, m_lastImageToProcess, QSize(m_ui->sb_size_w->value(),
                                              m_ui->sb_size_h->value()),
                                          m_ui->sigmaXSpinBox->value(), m_ui->sigmaYSpinBox->value());
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

std::tuple<QImage, quint64> GaussianBlurModel::processImage(const QImage image, const QSize& size,
                                                            const double sigmaX,
                                                            const double sigmaY) {
    QElapsedTimer timer;
    timer.start();
    const cv::Mat src = QImageToMat(image);
    cv::Mat dst;
    try {
        cv::GaussianBlur(src, dst, cv::Size(size.width(), size.height()), sigmaX, sigmaY);
    } catch (cv::Exception& e) {
        qDebug() << e.what();
        return std::make_tuple(QImage(), timer.elapsed());
    }
    const QImage result = MatToQImage(dst);
    return std::make_tuple(result, timer.elapsed());
}

QImage GaussianBlurModel::getImageToProcess() const {
    const auto lock = m_inImageData.lock();
    if (!lock) {
        return QImage();
    }
    return lock->image();
}
