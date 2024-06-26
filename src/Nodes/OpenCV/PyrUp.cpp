//
// Created by pablo on 3/11/24.
//

#include "PyrUp.h"
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include "Nodes/Conversor/MatQt.h"
#include <QtConcurrent/QtConcurrent>

PyrUp::PyrUp() {
    connect(&m_watcher, &QFutureWatcher<QPair<quint64, QImage>>::finished, this, &PyrUp::processFinished);
}

PyrUp::~PyrUp() {
}

QString PyrUp::caption() const {
    return QStringLiteral("Pyr Up");
}

QString PyrUp::name() const {
    return QStringLiteral("Pyr Up");
}

unsigned PyrUp::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType PyrUp::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
     switch (portType) {
        case QtNodes::PortType::In:
            if (portIndex == 0) {
                return ImageData().type();
            } else {
                return VariantData(QSize()).typeIn();
            }
        case QtNodes::PortType::Out:
            return ImageData().type();
        default:
            return ImageData().type();
    }
}

void PyrUp::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (m_inImageData.expired()) {
                m_outImageData.reset();
                m_spinBox->setValue(0);
            }
            break;
        case 1:
            m_inSizeData = std::dynamic_pointer_cast<VariantData>(nodeData);

            break;
        default:
            qCritical() << "Wrong port index";
    }
    if (const auto lock = m_inImageData.lock()) {
        m_lastImageToProcess = lock->image();
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> PyrUp::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* PyrUp::embeddedWidget() {
    if (!m_widget) {
        // Create inside the widget a horizontal layout with a label "Time ms:" and a spin box
        m_widget = new QWidget();
        auto verticalLayout = new QHBoxLayout(m_widget);
        auto label = new QLabel("Time ms:", m_widget);
        verticalLayout->addWidget(label);
        m_spinBox = new QSpinBox(m_widget);
        m_spinBox->setRange(0, 999999999);
        m_spinBox->setReadOnly(true);
        m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        verticalLayout->addWidget(m_spinBox);
    }
    return m_widget;
}

bool PyrUp::portCaptionVisible(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    return true;
}

QString PyrUp::portCaption(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    switch (port) {
        case QtNodes::PortType::In:
            if (port_index == 0) {
                return QStringLiteral("Image");
            } else {
                return QStringLiteral("[OPT]Scale factor (Size)");
            }
        case QtNodes::PortType::Out:
            return QStringLiteral("Image");
        default:
            return QStringLiteral("Image");
    }
}

QPair<quint64, QImage> PyrUp::processImage(const QImage& image, const QSize& size) {
    QElapsedTimer timer;
    timer.start();
    const cv::Mat src = QImageToMat(image);
    QImage result;
    try {
        cv::Mat dst;
        if (size.isEmpty()) {
            cv::pyrUp(src, dst);
        } else {
            const cv::Size cvSize(src.cols / size.width(), src.rows / size.height());
            cv::pyrUp(src, dst, cvSize);
        }
        result = MatToQImage(dst);
    } catch (cv::Exception& e) {
        const QString error = QString("PyrDown::processImage cv::Exception: %1").arg(e.what());
        qCritical() << error;
    }
    return {static_cast<quint64>(timer.elapsed()), result};
}

void PyrUp::requestProcess() {
if (m_processing) {
        return;
    }
    const auto lockImage = m_inImageData.lock();
    const auto lockSize = m_inSizeData.lock();
    if (!lockImage || m_lastImageToProcess.isNull()) {
        return;
    }
    const QSize size = lockSize ? lockSize->variant().toSize() : QSize();
    m_processing = true;
    const auto future = QtConcurrent::run(processImage, m_lastImageToProcess, size);
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

void PyrUp::processFinished() {
    m_processing = false;
    const auto [time, image] = m_watcher.result();
    m_spinBox->setValue(static_cast<int>(time));
    m_outImageData = std::make_shared<ImageData>(image);
    emit dataUpdated(0);
}
