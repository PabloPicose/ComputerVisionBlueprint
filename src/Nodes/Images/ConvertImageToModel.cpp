//
// Created by pablo on 3/2/24.
//

#include "ConvertImageToModel.h"
#include "ui_ConvertImageToForm.h"

#include <QElapsedTimer>
#include <QMetaEnum>
#include <QtConcurrent/QtConcurrentRun>

ConvertImageToModel::ConvertImageToModel() {
    connect(&m_watcher, &QFutureWatcher<QPair<QImage, quint64>>::finished, this, &ConvertImageToModel::processFinished);
}

ConvertImageToModel::~ConvertImageToModel() {
}

QString ConvertImageToModel::caption() const {
    return QString("Convert Image");
}

QString ConvertImageToModel::name() const {
    return QString("Convert Image");
}

unsigned ConvertImageToModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return 2;
        default:
            return 0;
    }
}

QtNodes::NodeDataType ConvertImageToModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return ImageFormatData().type();
                default:
                    return ImageData().type();
            }
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return ImageFormatData().type();
                default:
                    return ImageData().type();
            }
        default:
            return ImageData().type();
    }
}

void ConvertImageToModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            m_lastImageToProcess = getPixmapToProcess();
            break;
        case 1:
            m_inImageFormatData = std::dynamic_pointer_cast<ImageFormatData>(nodeData);
            m_lastFormatToProcess = getFormat();
            if (m_ui && !m_inImageFormatData.expired()) {
                m_ui->comboBox_format->setEnabled(false);
            } else if (m_ui) {
                m_ui->comboBox_format->setEnabled(true);
            }
            break;
        default:
            break;
    }
    requestProcess();
}

std::shared_ptr<QtNodes::NodeData> ConvertImageToModel::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outImageData;
        case 1:
            return m_outImageFormatData;
        default:
            return nullptr;
            break;
    }
}

QWidget* ConvertImageToModel::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::ConvertImageToForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        fillComboBoxFormats(m_ui->comboBox_format);
        connect(m_ui->comboBox_format, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &ConvertImageToModel::updateFormat);
        // connect the checkboxes to the flagsClicked();
        connect(m_ui->cb_auto_color, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_color_only, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_mono_only, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_diffuse_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_ordered_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_threshold_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_threshold_alpha_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_diffuse_alpha_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_ordered_alpha_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_auto_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_avoid_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_no_format_conversion, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_no_opaque_detection, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
        connect(m_ui->cb_prefer_dither, &QCheckBox::clicked, this, &ConvertImageToModel::flagsClicked);
    }
    return m_widget;
}

QJsonObject ConvertImageToModel::save() const {
    return NodeDelegateModel::save();
}

void ConvertImageToModel::load(QJsonObject const& jsonObj) {
    NodeDelegateModel::load(jsonObj);
}

QPair<QImage, quint64> ConvertImageToModel::processImage(const QImage& image, const QImage::Format format,
                                                         const QFlags<Qt::ImageConversionFlag> flags) {
    QElapsedTimer timer;
    timer.start();

    QImage newImage = image.convertToFormat(format, flags);
    return {newImage, static_cast<quint64>(timer.elapsed())};
}

void ConvertImageToModel::fillComboBoxFormats(QComboBox* comboBox) {
    // Fill the combobox with the QImage::Format enum values
    for (int i = 0; i < QImage::NImageFormats; ++i) {
        const auto format = static_cast<QImage::Format>(i);
        QString name = ImageFormatData::formatToString(format);
        // remove the Format_ prefix
        name.remove(0, 7);
        // replace the underscores with spaces
        name.replace("_", " ");
        comboBox->addItem(name, format);
    }
    comboBox->setCurrentIndex(comboBox->findData(QImage::Format::Format_Indexed8));
    comboBox->setMaxVisibleItems(5);
    comboBox->setStyleSheet("combobox-popup: 0;");
}

QImage::Format ConvertImageToModel::getFormat() const {
    const auto lock = m_inImageFormatData.lock();
    if (lock) {
        return lock->format();
    }
    return m_lastFormatToProcess;
}

void ConvertImageToModel::processFinished() {
    const auto [image, elapsed] = m_watcher.result();
    m_outImageData = std::make_shared<ImageData>(image);
    if (!m_inImageData.expired()) {
        m_outImageFormatData = std::make_shared<ImageFormatData>(getFormat());
    }
    if (m_ui) {
        m_ui->sb_time->setValue(elapsed);
    }
    emit dataUpdated(0);
    emit dataUpdated(1);
    m_processing = false;
    requestProcess();
}

void ConvertImageToModel::requestProcess() {
    if (m_lastImageToProcess.isNull()) {
        return;
    }
    if (m_processing) {
        return;
    }
    updateFlags();
    m_processing = true;

    const auto future = QtConcurrent::run(processImage, m_lastImageToProcess,
                                          m_lastFormatToProcess, m_flags);
    m_lastImageToProcess = QImage();
    m_watcher.setFuture(future);
}

QImage ConvertImageToModel::getPixmapToProcess() const {
    const auto lock = m_inImageData.lock();
    if (lock) {
        return lock->image();
    }
    return QImage();
}

void ConvertImageToModel::updateFlags() {
    // in the m_ui we have checkboxes to set the flags divided by category
    // color/mono = cb_auto_color, cb_color_only, cb_mono_only
    // dithering = cb_diffuse_dither, cb_ordered_dither, cb_threshold_dither
    // alpha = cb_threshold_alpha_dither, cb_diffuse_alpha_dither, cb_ordered_alpha_dither
    // color mathing= cb_auto_dither, cb_avoid_dither, cb_no_format_conversion, cb_no_opaque_detection, cb_prefer_dither

    // The checkboxes are auto exclusive
    m_flags = Qt::ImageConversionFlag::AutoColor;
    // color/mono
    if (m_ui->cb_auto_color->isChecked()) {
        m_flags = Qt::ImageConversionFlag::AutoColor;
    } else if (m_ui->cb_color_only->isChecked()) {
        m_flags = Qt::ImageConversionFlag::ColorOnly;
    } else if (m_ui->cb_mono_only->isChecked()) {
        m_flags = Qt::ImageConversionFlag::MonoOnly;
    }
    // dithering
    if (m_ui->cb_diffuse_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::DiffuseDither;
    } else if (m_ui->cb_ordered_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::OrderedDither;
    } else if (m_ui->cb_threshold_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::ThresholdDither;
    }
    // alpha
    if (m_ui->cb_threshold_alpha_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::ThresholdAlphaDither;
    } else if (m_ui->cb_diffuse_alpha_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::DiffuseAlphaDither;
    } else if (m_ui->cb_ordered_alpha_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::OrderedAlphaDither;
    }
    // color mathing
    if (m_ui->cb_auto_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::AutoDither;
    } else if (m_ui->cb_avoid_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::AvoidDither;
    } else if (m_ui->cb_no_format_conversion->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::NoFormatConversion;
    } else if (m_ui->cb_no_opaque_detection->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::NoOpaqueDetection;
    } else if (m_ui->cb_prefer_dither->isChecked()) {
        m_flags |= Qt::ImageConversionFlag::PreferDither;
    }
}

void ConvertImageToModel::updateFormat(int index) {
    const auto lockFormat = m_inImageFormatData.lock();
    if (lockFormat) {
        m_lastFormatToProcess = lockFormat->format();
    } else if (m_ui) {
        m_lastFormatToProcess = m_ui->comboBox_format->itemData(index).value<QImage::Format>();
    }
    m_lastImageToProcess = getPixmapToProcess();
    requestProcess();
}

void ConvertImageToModel::flagsClicked() {
    m_lastImageToProcess = getPixmapToProcess();
    requestProcess();
}
