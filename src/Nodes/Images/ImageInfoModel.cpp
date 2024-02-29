//
// Created by pablo on 2/29/24.
//

#include "ImageInfoModel.h"
#include "ui_ImageInfoForm.h"

ImageInfoModel::ImageInfoModel() {
}

ImageInfoModel::~ImageInfoModel() {
}

QString ImageInfoModel::caption() const {
    return QString("Image Info");
}

QString ImageInfoModel::name() const {
    return QString("Image Info");
}

unsigned ImageInfoModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 1;
        case QtNodes::PortType::Out:
            return 5;
        default:
            return 0;
            break;
    }
}

QtNodes::NodeDataType ImageInfoModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return ImageData().type();
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return ImageData().type();
                case 1:
                    return BooleanData().type();
                case 2:
                    return ImageFormatData().type();
                case 3:
                    return BooleanData().type();
                case 4:
                    return BooleanData().type();
                default:
                    break;
            }
        default:
            break;
    }
    return ImageData().type();
}

void ImageInfoModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
            updateData();
            break;
        }
        default:
            break;
    }
}

std::shared_ptr<QtNodes::NodeData> ImageInfoModel::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outImageData;
        case 1:
            return m_outIsNull;
        case 2:
            return m_outFormat;
        case 3:
            return m_outIsGrayscale;
        case 4:
            return m_outHasAlpha;
        default:
            return nullptr;
    }
}

QWidget* ImageInfoModel::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::ImageInfoForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
    }
    return m_widget;
}

void ImageInfoModel::invalidateOutData() {
    m_outImageData.reset();
    m_outIsNull.reset();
    m_outFormat.reset();
    m_outIsGrayscale.reset();
    m_outHasAlpha.reset();


}

void ImageInfoModel::updateData() {
    const auto imageLock = m_inImageData.lock();
    if (!imageLock) {
        invalidateOutData();
    } else {
        m_outImageData = std::make_shared<ImageData>(imageLock->image());
        m_outIsNull = std::make_shared<BooleanData>(imageLock->image().isNull());
        m_outFormat = std::make_shared<ImageFormatData>(imageLock->image().format());
        m_outIsGrayscale = std::make_shared<BooleanData>(imageLock->image().isGrayscale());
        m_outHasAlpha = std::make_shared<BooleanData>(imageLock->image().hasAlphaChannel());
        if (m_ui) {
            m_ui->cb_isNull->setChecked(m_outIsNull->boolean());
            m_ui->cb_isGrayScale->setChecked(m_outIsGrayscale->boolean());
            m_ui->cb_hasAlpha->setChecked(m_outHasAlpha->boolean());
            m_ui->le_format->setText(formatToString(m_outFormat->format()));
        }
    }
    emit dataUpdated(0);
    emit dataUpdated(1);
    emit dataUpdated(2);
    emit dataUpdated(3);
    emit dataUpdated(4);
}

QString ImageInfoModel::formatToString(QImage::Format const format) {
    switch (format) {
        case QImage::Format_Invalid: return "Invalid";
        case QImage::Format_Mono: return "Mono";
        case QImage::Format_MonoLSB: return "MonoLSB";
        case QImage::Format_Indexed8: return "Indexed8";
        case QImage::Format_RGB32: return "RGB32";
        case QImage::Format_ARGB32: return "ARGB32";
        case QImage::Format_ARGB32_Premultiplied: return "ARGB32 Premultiplied";
        case QImage::Format_RGB16: return "RGB16";
        case QImage::Format_ARGB8565_Premultiplied: return "ARGB8565 Premultiplied";
        case QImage::Format_RGB666: return "RGB666";
        case QImage::Format_ARGB6666_Premultiplied: return "ARGB6666 Premultiplied";
        case QImage::Format_RGB555: return "RGB555";
        case QImage::Format_ARGB8555_Premultiplied: return "ARGB8555 Premultiplied";
        case QImage::Format_RGB888: return "RGB888";
        case QImage::Format_RGB444: return "RGB444";
        case QImage::Format_ARGB4444_Premultiplied: return "ARGB4444 Premultiplied";
        case QImage::Format_RGBX8888: return "RGBX8888";
        case QImage::Format_RGBA8888: return "RGBA8888";
        case QImage::Format_RGBA8888_Premultiplied: return "RGBA8888 Premultiplied";
        case QImage::Format_BGR30: return "BGR30";
        case QImage::Format_A2BGR30_Premultiplied: return "A2BGR30 Premultiplied";
        case QImage::Format_RGB30: return "RGB30";
        case QImage::Format_A2RGB30_Premultiplied: return "A2RGB30 Premultiplied";
        case QImage::Format_Alpha8: return "Alpha8";
        case QImage::Format_Grayscale8: return "Grayscale8";
        // Include all other formats you need to handle
        default: return "Unknown Format";
    }
}
