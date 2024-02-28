#include "ImageLoaderModel.hpp"

#include <QtCore/QDir>
#include <QtCore/QEvent>

#include <QtWidgets/QFileDialog>

ImageLoaderModel::ImageLoaderModel()
    : _label(new QLabel("Double click to load image")) {
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    QFont f = _label->font();
    f.setBold(true);
    f.setItalic(true);

    _label->setFont(f);

    _label->setMinimumSize(200, 200);
    //_label->setMaximumSize(500, 300);

    _label->installEventFilter(this);
}

unsigned int ImageLoaderModel::nPorts(const QtNodes::PortType portType) const {
    unsigned int result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 0;
            break;

        case QtNodes::PortType::Out:
            result = 1;

        default:
            break;
    }

    return result;
}

bool ImageLoaderModel::eventFilter(QObject* object, QEvent* event) {
    if (object == _label) {
        const int w = _label->width();
        const int h = _label->height();

        if (event->type() == QEvent::MouseButtonPress) {
            const QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                                  tr("Open Image"),
                                                                  QDir::homePath(),
                                                                  tr("Image Files (*.png *.jpg *.bmp)"));

            m_outPixmapData = std::make_shared<PixmapData>(fileName);
            if (!m_outPixmapData->isNull()) {
                _label->setPixmap(m_outPixmapData->pixmap().scaled(w, h, Qt::KeepAspectRatio));
            }

            Q_EMIT dataUpdated(0);

            return true;
        } else if (event->type() == QEvent::Resize) {
            if (m_outPixmapData && !m_outPixmapData->isNull())
                _label->setPixmap(m_outPixmapData->pixmap().scaled(w, h, Qt::KeepAspectRatio));
        }
    }

    return false;
}

NodeDataType ImageLoaderModel::dataType(QtNodes::PortType const, QtNodes::PortIndex const) const {
    return PixmapData().type();
}

std::shared_ptr<NodeData> ImageLoaderModel::outData(QtNodes::PortIndex) {
    return m_outPixmapData;
}
