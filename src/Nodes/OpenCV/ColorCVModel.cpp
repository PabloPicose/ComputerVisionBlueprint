//
// Created by pablo on 2/24/24.
//

#include "ColorCVModel.h"
#include <QComboBox>

#include "Nodes/Data/ImageData.h"
#include "Nodes/Conversor/MatQt.h"

ColorCVModel::ColorCVModel() {
}

ColorCVModel::~ColorCVModel() {
    delete m_comboBox;
    m_comboBox = nullptr;
}

QString ColorCVModel::caption() const {
    return "Color OpenCV";
}

QString ColorCVModel::name() const {
    return "ColorCV";
}

unsigned ColorCVModel::nPorts(QtNodes::PortType portType) const {
    unsigned result = 1;

    switch (portType) {
        case QtNodes::PortType::In:
            result = 1;
            break;

        case QtNodes::PortType::Out:
            result = 1;
            break;

        default:
            break;
    }
    return result;
}

QtNodes::NodeDataType ColorCVModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}

void ColorCVModel::setInData(std::shared_ptr<QtNodes::NodeData> const nodeData, const QtNodes::PortIndex portIndex) {
    m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
    const auto lockData = m_inImageData.lock();
    if (lockData) {
        const auto image = lockData->image();
        m_outImageData = std::make_shared<ImageData>(processImage(image, m_comboBox->currentData().toInt()));
    } else {
        m_outImageData.reset();
    }
    Q_EMIT dataUpdated(0);
}

std::shared_ptr<QtNodes::NodeData> ColorCVModel::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* ColorCVModel::embeddedWidget() {
    if (!m_comboBox) {
        m_comboBox = new QComboBox();
        m_comboBox->addItem("BGR to GRAY", cv::COLOR_BGR2GRAY);
        m_comboBox->addItem("GRAY to BGR", cv::COLOR_GRAY2BGR);
        m_comboBox->setMaximumSize(m_comboBox->sizeHint());
        cv::COLOR_RGB2GRAY;

        connect(m_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                [=](int) {
                    Q_EMIT dataUpdated(0);
                });
    }
    return m_comboBox;
}

QImage ColorCVModel::processImage(QImage image, const int code) {
    if (image.isNull()) {
        return QImage();
    }
    const auto mat = QImageToMat(image);
    cv::Mat out;
    cv::cvtColor(mat, out, code);

    return MatToQImage(out);
}
