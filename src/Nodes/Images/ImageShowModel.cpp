#include "ImageShowModel.hpp"


#include <QtNodes/NodeDelegateModelRegistry>

#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtWidgets/QFileDialog>

ImageShowModel::ImageShowModel()
    : _label(new QLabel("Image will appear here")) {
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    QFont f = _label->font();
    f.setBold(true);
    f.setItalic(true);

    _label->setFont(f);

    _label->setMinimumSize(200, 200);

    _label->installEventFilter(this);
}

unsigned int ImageShowModel::nPorts(const QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 1;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

bool ImageShowModel::eventFilter(QObject* object, QEvent* event) {
    if (object == _label) {
        int w = _label->width();
        int h = _label->height();

        if (event->type() == QEvent::Resize) {
            const auto d = m_outData.lock();
            if (d) {
                if (!d->pixmap().isNull())
                    _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
            }
        }
    }

    return false;
}

QtNodes::NodeDataType ImageShowModel::dataType(QtNodes::PortType const, QtNodes::PortIndex const) const {
    return ImageData().type();
}

std::shared_ptr<QtNodes::NodeData> ImageShowModel::outData(QtNodes::PortIndex) {
    return m_outData.lock();
}

void ImageShowModel::setInData(const std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const) {
    m_outData = std::dynamic_pointer_cast<ImageData>(nodeData);
    const auto lockData = m_outData.lock();

    if (lockData) {

        const int w = _label->width();
        const int h = _label->height();

        if (!lockData->pixmap().isNull())
            _label->setPixmap(lockData->pixmap().scaled(w, h, Qt::KeepAspectRatio));
    } else {
        _label->setPixmap(QPixmap());
    }

    Q_EMIT dataUpdated(0);
}
