//
// Created by pablo on 3/1/24.
//

#include "DetectMultiScaleModel.h"
#include "ui_DetectMultiScaleForm.h"
#include "Nodes/Conversor/MatQt.h"

#include <QtConcurrent/QtConcurrent>

DetectMultiScaleModel::DetectMultiScaleModel() {
    connect(&m_watcher, &QFutureWatcher<QPair<QList<QRect>, quint64>>::finished, this,
            &DetectMultiScaleModel::processFinished);
}

DetectMultiScaleModel::~DetectMultiScaleModel() {
}

QString DetectMultiScaleModel::caption() const {
    return QString("Detect Multi Scale");
}

QString DetectMultiScaleModel::name() const {
    return QString("Detect Multi Scale");
}

unsigned DetectMultiScaleModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 6;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType DetectMultiScaleModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return CascadeClassifierData().type();
                case 2:
                    return VariantData(0.0).typeIn();
                case 3:
                    return VariantData(0).typeIn();
                case 4:
                    return VariantData(QSize()).typeIn();
                case 5:
                    return VariantData(QSize()).typeIn();
                default:
                    break;
            }
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                default:
                    break;
            }

        default:
            return FileData().type();
    }
}

void DetectMultiScaleModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (m_inImageData.expired()) {
                m_lastImageToProcess = QImage();
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }
            break;
        }
        case 1: {
            m_inFileData = std::dynamic_pointer_cast<CascadeClassifierData>(nodeData);
            if (m_inFileData.expired()) {
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }
            break;
        }
        case 2: {
            m_inScaleFactorData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (m_ui && !m_inScaleFactorData.expired()) {
                m_ui->sb_scaleFactor->setEnabled(false);
            } else if (m_ui) {
                m_ui->sb_scaleFactor->setEnabled(true);
            }
            break;
        }
        case 3: {
            m_inMinNeighborsData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (m_ui && !m_inMinNeighborsData.expired()) {
                m_ui->sb_minNeightbors->setEnabled(false);
            } else if (m_ui) {
                m_ui->sb_minNeightbors->setEnabled(true);
            }
            break;
        }
        case 4: {
            m_inMinSizeData = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (m_ui && !m_inMinSizeData.expired()) {
                m_ui->sb_minSizeW->setEnabled(false);
                m_ui->sb_minSizeH->setEnabled(false);
            } else if (m_ui) {
                m_ui->sb_minSizeW->setEnabled(true);
                m_ui->sb_minSizeH->setEnabled(true);
            }
            break;
        }
        case 5: {
            m_inMaxSizeData = std::dynamic_pointer_cast<VariantData>(nodeData);
            break;
        }
        default:
            break;
    }
    updateInData();
}

std::shared_ptr<QtNodes::NodeData> DetectMultiScaleModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* DetectMultiScaleModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = std::make_unique<Ui::DetectMultiScaleForm>();
        m_ui->setupUi(m_widget);
        connect(m_ui->sb_scaleFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double value) {
            m_inScaleFactor = value;
            updateInData();
        });
        connect(m_ui->sb_minNeightbors, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_inMinNeighbors = value;
            updateInData();
        });
        connect(m_ui->sb_minSizeW, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_inMinSize.setWidth(value);
            updateInData();
        });
        connect(m_ui->sb_minSizeH, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_inMinSize.setHeight(value);
            updateInData();
        });
        connect(m_ui->sb_maxSizeW, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_inMaxSize.setWidth(value);
            updateInData();
        });
        connect(m_ui->sb_maxSizeH, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            m_inMaxSize.setHeight(value);
            updateInData();
        });
    }
    return m_widget;
}

QString DetectMultiScaleModel::portCaption(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    switch (port) {
        case QtNodes::PortType::In:
            return QStringLiteral("Cascade Classifier");
        case QtNodes::PortType::Out:
            switch (port_index) {
                case 0:
                    return QStringLiteral("Output");
                default:
                    break;
            }
        default:
            break;
    }
    return NodeDelegateModel::portCaption(port, port_index);
}

void DetectMultiScaleModel::processFinished() {
    const auto [rects, time] = m_watcher.result();
    m_outImageData = std::make_shared<RectsData>(rects);
    if (m_ui) {
        m_ui->sb_time->setValue(time);
    }
    emit dataUpdated(0);
    requestProcess();
}

QPair<QList<QRect>, quint64> DetectMultiScaleModel::processMultiScale(const QImage& image, cv::CascadeClassifier cc,
                                                                      double scaleFactor,
                                                                      int minNeighbors, int flags, const QSize& minSize,
                                                                      const QSize& maxSize) {
    QElapsedTimer timer;
    timer.start();
    const cv::Mat src = QImageToMat(image);
    QList<QRect> qRects;
    try {
        std::vector<cv::Rect> objects;
        const int minW = minSize.width();
        const int minH = minSize.height();
        const int maxW = maxSize.width();
        const int maxH = maxSize.height();

        const cv::Size minSizeCv(minSize.width(), minSize.height());
        const cv::Size maxSizeCv(maxSize.width(), maxSize.height());
        cc.detectMultiScale(src, objects, scaleFactor, minNeighbors, flags, minSizeCv, maxSizeCv);
        for (const auto& r: objects) {
            qRects.append(QRect(r.x, r.y, r.width, r.height));
        }
    } catch (cv::Exception& e) {
        qDebug() << e.what();
    }
    qDebug() << "Detected rects:" << qRects;
    return {qRects, timer.elapsed()};
}

void DetectMultiScaleModel::requestProcess() {
    if (m_lastImageToProcess.isNull()) {
        return;
    }
    if (m_watcher.isRunning()) {
        return;
    }
    const auto lockCascade = m_inFileData.lock();
    if (!lockCascade) {
        return;
    }

    const cv::CascadeClassifier cc = *lockCascade->cascadeClassifier().get();
    const auto future = QtConcurrent::run(&DetectMultiScaleModel::processMultiScale,
                                          m_lastImageToProcess,
                                          cc,
                                          m_inScaleFactor,
                                          m_inMinNeighbors,
                                          0,
                                          m_inMinSize,
                                          m_inMaxSize
    );
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

void DetectMultiScaleModel::updateInData() {
    m_lastImageToProcess = getLatestImageToProcess();
    const auto lockScaleFator = m_inScaleFactorData.lock();
    if (lockScaleFator && lockScaleFator->metaType() == QMetaType::Double) {
        m_inScaleFactor = lockScaleFator->variant().toDouble();
        if (m_ui) {
            QSignalBlocker sb(m_ui->sb_scaleFactor);
            m_ui->sb_scaleFactor->setValue(m_inScaleFactor);
        }
    }
    const auto lockMinNeighbors = m_inMinNeighborsData.lock();
    if (lockMinNeighbors && lockMinNeighbors->metaType() == QMetaType::Int) {
        m_inMinNeighbors = lockMinNeighbors->variant().toInt();
        if (m_ui) {
            QSignalBlocker sb(m_ui->sb_minNeightbors);
            m_ui->sb_minNeightbors->setValue(m_inMinNeighbors);
        }
    }
    const auto lockMinSize = m_inMinSizeData.lock();
    if (lockMinSize && lockMinSize->metaType() == QMetaType::QSize) {
        m_inMinSize = lockMinSize->variant().toSize();
        if (m_ui) {
            QSignalBlocker sbW(m_ui->sb_minSizeW);
            QSignalBlocker sbH(m_ui->sb_minSizeH);
            m_ui->sb_minSizeW->setValue(m_inMinSize.width());
            m_ui->sb_minSizeH->setValue(m_inMinSize.height());
        }
    }
    const auto lockMaxSize = m_inMaxSizeData.lock();
    if (lockMaxSize && lockMaxSize->metaType() == QMetaType::QSize) {
        m_inMaxSize = lockMaxSize->variant().toSize();
        if (m_ui) {
            QSignalBlocker sbW(m_ui->sb_maxSizeW);
            QSignalBlocker sbH(m_ui->sb_maxSizeH);
            m_ui->sb_maxSizeW->setValue(m_inMaxSize.width());
            m_ui->sb_maxSizeH->setValue(m_inMaxSize.height());
        }
    }
    requestProcess();
}

QImage DetectMultiScaleModel::getLatestImageToProcess() const {
    const auto lock = m_inImageData.lock();
    if (lock) {
        return lock->image();
    }
    return QImage();
}
