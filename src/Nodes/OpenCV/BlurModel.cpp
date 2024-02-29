//
// Created by pablo on 2/24/24.
//

#include "BlurModel.h"

#include <QSpinBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <Nodes/Data/DataInclude.h>
#include <opencv2/opencv.hpp>
#include "Nodes/Conversor/MatQt.h"
#include <QtConcurrent/QtConcurrent>

#include "ui_BlurForm.h"


BlurModel::BlurModel() {
    connect(&m_watcher, &QFutureWatcher<QPixmap>::finished, this, &BlurModel::processFinished);
}

BlurModel::~BlurModel() {
    if (m_widget)
        m_widget->deleteLater();
}

QString BlurModel::caption() const {
    return QString("Blur");
}

QString BlurModel::name() const {
    return QString("Blur");
}

unsigned BlurModel::nPorts(QtNodes::PortType portType) const {
    unsigned result = 1;
    switch (portType) {
        case QtNodes::PortType::In:
            result = 2;
            break;
        case QtNodes::PortType::Out:
            result = 1;
            break;
        default:
            break;
    }
    return result;
}

QtNodes::NodeDataType BlurModel::dataType(QtNodes::PortType const portType, QtNodes::PortIndex const portIndex) const {
    if (portType == QtNodes::PortType::In) {
        if (portIndex == 0) {
            return ImageData().type();
        } else {
            return VariantData().typeIn();
        }
    } else {
        return ImageData().type();
    }
}

void BlurModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (!m_inImageData.lock()) {
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }else {
                m_lastImageToProcess = getImageToProcess();
            }
        }
        break;
        case 1: {
            const auto sizeDataPtr = std::dynamic_pointer_cast<VariantData>(nodeData);
            if (!sizeDataPtr) {
                m_ui->sb_width->setEnabled(true);
                m_ui->sb_height->setEnabled(true);
            } else {
                QSignalBlocker blocker(m_ui->sb_width);
                QSignalBlocker blocker2(m_ui->sb_height);
                const QSize size = sizeDataPtr->variant().toSize();
                m_ui->sb_width->setValue(size.width());
                m_ui->sb_height->setValue(size.height());
                m_ui->sb_width->setEnabled(false);
                m_ui->sb_height->setEnabled(false);
            }
        }
        break;
        default:
            qCritical() << "Invalid IN port index";
            break;
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> BlurModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* BlurModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = new Ui::BlurForm();
        m_ui->setupUi(m_widget);

        // connect sb_width QSpinBox to signal valueChanged
        connect(m_ui->sb_width, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            if (value > 0) {
                m_ui->lbl_warn_width->setVisible(false);
            } else {
                m_ui->lbl_warn_width->setVisible(true);
            }
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        // connect sb_height QSpinBox to signal valueChanged
        connect(m_ui->sb_height, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            if (value > 0) {
                m_ui->lbl_warn_height->setVisible(false);
            } else {
                m_ui->lbl_warn_height->setVisible(true);
            }
            m_lastImageToProcess = getImageToProcess();
            requestProcess();
        });
        m_widget->setMaximumSize(m_widget->sizeHint());
    }
    return m_widget;
}

QImage BlurModel::processImage(const QSize& size, const QImage& image) {
    const int width = size.width();
    const int height = size.height();
    QImage result;
    try {
        const auto mat = QImageToMat(image);
        cv::Mat matResult;
        cv::blur(mat, matResult, cv::Size(width, height));
        result = MatToQImage(matResult);
    } catch (const std::exception& e) {
        qCritical() << e.what();
    }
    return result;
}

QImage BlurModel::getImageToProcess() const {
    const auto imagePtr = m_inImageData.lock();
    if (!imagePtr) {
        return QImage();
    }
    return imagePtr->image();
}

void BlurModel::processFinished() {
    m_outImageData = std::make_shared<ImageData>(m_watcher.result());
    Q_EMIT dataUpdated(0);
    requestProcess();
}

void BlurModel::requestProcess() {
    if (m_watcher.isRunning() || m_lastImageToProcess.isNull()) {
        return;
    }
    const auto future = QtConcurrent::run(processImage,
                                          QSize(m_ui->sb_width->value(), m_ui->sb_height->value()),
                                           m_lastImageToProcess);
    m_lastImageToProcess= QImage();
    m_watcher.setFuture(future);
}
