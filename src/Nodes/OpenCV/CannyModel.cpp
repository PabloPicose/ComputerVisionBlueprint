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
    m_pixmap = new QPixmap();
    connect(&m_watcher, &QFutureWatcher<QPixmap>::finished, this, &CannyModel::processFinished);
}

CannyModel::~CannyModel() {
    delete m_pixmap;
    m_pixmap = nullptr;
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
                return PixmapData().type();
            case 1:
                return DecimalData().type();
            case 2:
                return DecimalData().type();
            case 3:
                return NumericalData().type();
            case 4:
                return BooleanData().type();
            default:
                qCritical() << "Invalid port index";
                return PixmapData().type();
                break;
        }
    }
    return PixmapData().type();
}

void CannyModel::setInData(std::shared_ptr<QtNodes::NodeData> const nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inPixmapData = std::dynamic_pointer_cast<PixmapData>(nodeData);
            if (!m_inPixmapData) {
                m_inPixmapData = std::make_shared<PixmapData>();
            }
            break;
        case 1:
            m_ui->sb_threshold->setValue(std::dynamic_pointer_cast<DecimalData>(nodeData)->number());
            break;
        case 2:
            m_ui->sb_threshold2->setValue(std::dynamic_pointer_cast<DecimalData>(nodeData)->number());
            break;
        case 3:
            m_ui->sb_apertureSize->setValue(std::dynamic_pointer_cast<NumericalData>(nodeData)->number());
            break;
        case 4:
            m_ui->cb_gradient->setChecked(std::dynamic_pointer_cast<BooleanData>(nodeData)->boolean());
            break;
        default:
            qCritical() << "Invalid port index";
            break;
    }

    requestCompute();
}

std::shared_ptr<QtNodes::NodeData> CannyModel::outData(const QtNodes::PortIndex port) {
    return std::make_shared<PixmapData>(*m_pixmap);
}

QWidget* CannyModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = new Ui::CannyForm();
        m_ui->setupUi(m_widget);
        // connect the QDoubleSpinBox sb_threshold, sb_threshold2
        connect(m_ui->sb_threshold, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                &CannyModel::requestCompute);
        connect(m_ui->sb_threshold2, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
                &CannyModel::requestCompute);
        // connect the QSpinBox sb_apertureSize
        connect(m_ui->sb_apertureSize, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
            // value odd between 3 and 7 convert to 3 or 5 or 7
                if (value % 2 == 0) {
                    m_ui->sb_apertureSize->setValue(value + 1);
                } else {
                    requestCompute();
                }
        });
        // connect the QCheckBox cb_L2gradient
        connect(m_ui->cb_gradient, &QCheckBox::stateChanged, this, &CannyModel::requestCompute);
    }
    return m_widget;
}

void CannyModel::requestCompute() {
    if (m_needToCompute) {
        return;
    }
    m_needToCompute = true;
    runCanny();
}

void CannyModel::processFinished() {
    *m_pixmap = m_watcher.result();
    Q_EMIT dataUpdated(0);
    if (m_needToCompute) {
        runCanny();
    }
}

QPixmap CannyModel::processImage(const QPixmap& pixmap, const double lowThreshold, const double highThreshold,
                                 const int apertureSize,
                                 const bool useL2Gradient) {
    const cv::Mat src = QPixmapToMat(pixmap);
    cv::Mat dst;
    try {
        cv::Canny(src, dst, lowThreshold, highThreshold, apertureSize, useL2Gradient);
    } catch (cv::Exception& e) {
        qDebug() << e.what();
        return QPixmap();
    }
    return MatToQPixmap(dst);
}

void CannyModel::runCanny() {
    if (!m_watcher.isRunning()) {
        m_needToCompute = false;
        // copy the values from the UI and run the processImage function
        const auto pixmap = *m_pixmap;
        const auto lowThreshold = m_ui->sb_threshold->value();
        const auto highThreshold = m_ui->sb_threshold2->value();
        const auto kernelSize = m_ui->sb_apertureSize->value();
        const auto useL2Gradient = m_ui->cb_gradient->isChecked();
        auto future = QtConcurrent::run([this, pixmap, lowThreshold, highThreshold, kernelSize, useL2Gradient]() {
            return processImage(pixmap, lowThreshold, highThreshold, kernelSize, useL2Gradient);
        });
        m_watcher.setFuture(future);
    }
}
