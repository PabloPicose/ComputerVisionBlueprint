//
// Created by pablo on 2/29/24.
//

#include "DrawLinesModel.h"

#include <QPainter>
#include <QPen>
#include <QElapsedTimer>
#include <QtConcurrent/QtConcurrent>

#include "ui_DrawLinesForm.h"

DrawLinesModel::DrawLinesModel() {
    connect(&m_watcher, &QFutureWatcher<QPair<QImage, quint64>>::finished, this, &DrawLinesModel::processFinished);
}

DrawLinesModel::~DrawLinesModel() {
}

QString DrawLinesModel::caption() const {
    return QStringLiteral("Draw Lines");
}

QString DrawLinesModel::name() const {
    return QStringLiteral("Draw Lines");
}

unsigned DrawLinesModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 4;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType DrawLinesModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return LinesSegmentData().type();
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

void DrawLinesModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            if (m_inImageData.expired()) {
                m_lastPixmapToProcess = QImage();
                m_outImageData.reset();
                emit dataUpdated(0);
                return;
            }
            break;
        case 1:
            m_inLinesSegmentData = std::dynamic_pointer_cast<LinesSegmentData>(nodeData);
            if (m_inLinesSegmentData.expired()) {
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

std::shared_ptr<QtNodes::NodeData> DrawLinesModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* DrawLinesModel::embeddedWidget() {
    if (!m_widget) {
        m_ui.reset(new Ui::DrawLinesForm);
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        // sb_r sb_g sb_b, sb_thickness
        connect(m_ui->sb_r, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_lastColor.setRed(value);
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_g, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_lastColor.setGreen(value);
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_b, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_lastColor.setBlue(value);
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
        connect(m_ui->sb_thickness, QOverload<int>::of(&QSpinBox::valueChanged), [this](int value) {
            m_lastThickness = value;
            m_lastPixmapToProcess = getPixmapToProcess();
            requestProcess();
        });
    }
    return m_widget;
}

void DrawLinesModel::processFinished() {
    const auto [fst, snd] = m_watcher.result();
    m_outImageData = std::make_shared<ImageData>(fst);
    m_ui->sb_time->setValue(snd);

    emit dataUpdated(0);
    requestProcess();
}

void DrawLinesModel::requestProcess() {
    if (m_inImageData.expired()) {
        return;
    }
    if (m_inLinesSegmentData.expired()) {
        return;
    }
    if (m_watcher.isRunning()) {
        return;
    }
    if (m_lastPixmapToProcess.isNull()) {
        return;
    }
    const auto future = QtConcurrent::run(&DrawLinesModel::processImage, m_lastPixmapToProcess,
                                          m_inLinesSegmentData.lock()->lines(), m_lastColor, m_lastThickness);
    m_lastPixmapToProcess = QImage();
    m_watcher.setFuture(future);
}

QImage DrawLinesModel::getPixmapToProcess() const {
    const auto lock = m_inImageData.lock();
    if (lock) {
        return lock->image();
    }
    return QImage();
}

QPair<QImage, quint64> DrawLinesModel::processImage(QImage image, const LinesSegment& linesSegment, const QColor& color,
                                                    const int thickness) {
    QElapsedTimer timer;
    timer.start();
    QPainter painter(&image);

    const QPen pen(color, thickness);
    painter.setPen(pen);

    painter.drawLines(linesSegment);
    painter.end();

    return {image, static_cast<quint64>(timer.elapsed())};
}

void DrawLinesModel::updateFromInputPort() {
    const auto lockImage = m_inImageData.lock();
    if (lockImage) {
        m_lastPixmapToProcess = lockImage->image();
    }
    const auto lockColor = m_inColor.lock();
    if (lockColor) {
        QSignalBlocker sbr(m_ui->sb_r);
        QSignalBlocker sbg(m_ui->sb_g);
        QSignalBlocker sbb(m_ui->sb_b);
        m_lastColor = lockColor->variant().value<QColor>();
        m_ui->sb_r->setEnabled(false);
        m_ui->sb_g->setEnabled(false);
        m_ui->sb_b->setEnabled(false);
        m_ui->sb_r->setValue(m_lastColor.red());
        m_ui->sb_g->setValue(m_lastColor.green());
        m_ui->sb_b->setValue(m_lastColor.blue());
    } else {
        m_ui->sb_r->setEnabled(true);
        m_ui->sb_g->setEnabled(true);
        m_ui->sb_b->setEnabled(true);
    }
    const auto lockThickness = m_inThickness.lock();
    if (lockThickness && lockThickness->metaType() == QMetaType::Int) {
        QSignalBlocker sbt(m_ui->sb_thickness);
        m_lastThickness = lockThickness->variant().toInt();
        m_ui->sb_thickness->setEnabled(false);
        m_ui->sb_thickness->setValue(m_lastThickness);
    } else {
        m_ui->sb_thickness->setEnabled(true);
    }
    requestProcess();
}
