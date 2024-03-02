//
// Created by pablo on 3/1/24.
//

#include "EqualizeHistModel.h"

#include "Nodes/Conversor/MatQt.h"
#include <QtConcurrent/QtConcurrent>
#include <QLabel>

EqualizeHistModel::EqualizeHistModel() {
    connect(&m_watcher, &QFutureWatcher<QPair<QImage, quint64>>::finished, this, &EqualizeHistModel::processFinished);
}

EqualizeHistModel::~EqualizeHistModel() {
}

QString EqualizeHistModel::caption() const {
    return "Equalize Hist";
}

QString EqualizeHistModel::name() const {
    return "Equalize Hist";
}

unsigned EqualizeHistModel::nPorts(QtNodes::PortType portType) const {
    return 1;
}

QtNodes::NodeDataType EqualizeHistModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}

void EqualizeHistModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (m_inImageData.expired()) {
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }
            m_lastImageToProcess = m_inImageData.lock()->image();
            requestProcess();
            break;
        default:
            break;
    }
}

std::shared_ptr<QtNodes::NodeData> EqualizeHistModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* EqualizeHistModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QLabel("Time: 0 ms  ");
    }
    return m_widget;
}

QString EqualizeHistModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return QStringLiteral("Image 8-bit");
        default:
            break;
    }
    return NodeDelegateModel::portCaption(portType, portIndex);
}

bool EqualizeHistModel::portCaptionVisible(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    switch (port) {
        case QtNodes::PortType::In:
            return true;
        default:
            return NodeDelegateModel::portCaptionVisible(port, port_index);
            break;
    }
}

void EqualizeHistModel::processFinished() {
    const auto [image, time] = m_watcher.result();
    if (!m_inImageData.expired()) {
        m_outImageData = std::make_shared<ImageData>(image);
        m_widget->setText(QString("Time: %1 ms").arg(time));
        emit dataUpdated(0);
    }
    m_widget->adjustSize();
    requestProcess();
}

void EqualizeHistModel::requestProcess() {
    if (m_watcher.isRunning())
        return;
    if (m_lastImageToProcess.isNull())
        return;
    if (m_inImageData.expired())
        return;

    const auto future = QtConcurrent::run(equializeHist, m_lastImageToProcess);
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

QPair<QImage, quint64> EqualizeHistModel::equializeHist(const QImage& image) {
    QElapsedTimer timer;
    timer.start();
    const cv::Mat mat = QImageToMat(image);
    QImage result;
    try {
        cv::Mat dst;
        cv::equalizeHist(mat, dst);
        result = MatToQImage(dst);
    } catch (cv::Exception& e) {
        qDebug() << e.what();
    }
    return qMakePair(result, timer.elapsed());
}
