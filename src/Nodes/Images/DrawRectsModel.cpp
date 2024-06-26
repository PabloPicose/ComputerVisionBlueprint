//
// Created by pablo on 3/1/24.
//

#include "DrawRectsModel.h"
#include "ui_DrawRectsForm.h"
#include <QPainter>
#include <QPen>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

DrawRectsModel::DrawRectsModel() {
    connect(&m_watcher, &QFutureWatcher<QPair<QImage, quint64>>::finished, this, &DrawRectsModel::processFinished);
}

DrawRectsModel::~DrawRectsModel() {
}

QString DrawRectsModel::caption() const {
    return "Draw Rects";
}

QString DrawRectsModel::name() const {
    return "Draw Rects";
}

unsigned DrawRectsModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 4;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType DrawRectsModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return RectsData().type();
                case 2:
                    return VariantData(QColor()).typeIn();
                case 3:
                    return VariantData(0).typeIn();
                default:
                    return VariantData().type();
            }
        case QtNodes::PortType::Out:
            return ImageData().type();
        default:
            return VariantData().type();
    }
}

void DrawRectsModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (m_inImageData.expired()) {
                m_lastPixmapToProcess = QImage();
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            } else {
                m_lastPixmapToProcess = m_inImageData.lock()->image();
            }
            break;
        case 1:
            m_inRectsData = std::dynamic_pointer_cast<RectsData>(nodeData);
            if (m_inRectsData .expired()) {
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }
            break;
        case 2:
            m_inColor = std::dynamic_pointer_cast<VariantData>(nodeData);
            break;
        case 3:
            m_inThickness = std::dynamic_pointer_cast<VariantData>(nodeData);
            break;
        default:
            break;
    }
    updateFromInputPort();
}

std::shared_ptr<QtNodes::NodeData> DrawRectsModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* DrawRectsModel::embeddedWidget() {

    if (!m_widget) {
        m_ui.reset(new Ui::DrawRectsForm());
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        // sb_r sb_g sb_b, sb_thickness
        connect(m_ui->sb_r, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_color.setRed(value);
            updateFromInputPort();
        });
        connect(m_ui->sb_g, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_color.setGreen(value);
            updateFromInputPort();
        });
        connect(m_ui->sb_b, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_color.setBlue(value);
            updateFromInputPort();
        });
        connect(m_ui->sb_thickness, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_thickness = value;
            updateFromInputPort();
        });
    }
    return m_widget;
}

QPair<QImage, quint64> DrawRectsModel::processImage(QImage image, const Rects& rects, const QColor& color,
                                                    int thickness) {
    QElapsedTimer timer;
    timer.start();
    QPainter painter(&image);

    const QPen pen(color, thickness);
    painter.setPen(pen);

    painter.drawRects(rects);

    painter.end();

    return {image, static_cast<quint64>(timer.elapsed())};
}

void DrawRectsModel::updateFromInputPort() {
    m_lastPixmapToProcess = getPixmapToProcess();
    const auto lockColor = m_inColor.lock();
    if (lockColor) {
        QSignalBlocker sbr(m_ui->sb_r);
        QSignalBlocker sbg(m_ui->sb_g);
        QSignalBlocker sbb(m_ui->sb_b);
        m_color = lockColor->variant().value<QColor>();
        m_ui->sb_r->setEnabled(false);
        m_ui->sb_g->setEnabled(false);
        m_ui->sb_b->setEnabled(false);
        m_ui->sb_r->setValue(m_color.red());
        m_ui->sb_g->setValue(m_color.green());
        m_ui->sb_b->setValue(m_color.blue());
    } else {
        m_ui->sb_r->setEnabled(true);
        m_ui->sb_g->setEnabled(true);
        m_ui->sb_b->setEnabled(true);
    }
    const auto lockThickness = m_inThickness.lock();
    if (lockThickness && lockThickness->metaType() == QMetaType::Int) {
        QSignalBlocker sbt(m_ui->sb_thickness);
        m_thickness = lockThickness->variant().toInt();
        m_ui->sb_thickness->setEnabled(false);
        m_ui->sb_thickness->setValue(m_thickness);
    } else {
        m_ui->sb_thickness->setEnabled(true);
    }
    requestProcess();
}

QImage DrawRectsModel::getPixmapToProcess() const {
    const auto lock = m_inImageData.lock();
    if (lock) {
        return lock->image();
    }
    return QImage();
}

void DrawRectsModel::processFinished() {
    const auto [fst, snd] = m_watcher.result();
    m_outImageData = std::make_shared<ImageData>(fst);
    m_ui->sb_time->setValue(snd);

    emit dataUpdated(0);
    requestProcess();
}

void DrawRectsModel::requestProcess() {
    if (m_inImageData.expired()) {
        return;
    }
    if (m_inRectsData.expired()) {
        return;
    }
    if (m_watcher.isRunning()) {
        return;
    }
    if (m_lastPixmapToProcess.isNull()) {
        return;
    }
    /*const auto future = QtConcurrent::run(&DrawRectsModel::processImage, m_lastPixmapToProcess,
                                          m_inRectsData.lock()->rects(), m_color, m_thickness); */
    //m_watcher.setFuture(future);
    const auto [fst, snd] =  processImage(m_lastPixmapToProcess, m_inRectsData.lock()->rects(), m_color, m_thickness);
    if (m_ui) {
        m_ui->sb_time->setValue(snd);
    }
    m_outImageData = std::make_shared<ImageData>(fst);
    m_lastPixmapToProcess = QImage();

    emit dataUpdated(0);
}
