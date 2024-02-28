//
// Created by pablo on 2/24/24.
//

#include "ColorCV.h"
#include <QComboBox>

#include "Nodes/NodesInclude.h"
#include "Nodes/Data/ImageData.h"

ColorCV::ColorCV() {
}

ColorCV::~ColorCV() {
    delete m_comboBox;
    m_comboBox = nullptr;
}

QString ColorCV::caption() const {
    return "Color OpenCV";
}

QString ColorCV::name() const {
    return "ColorCV";
}

unsigned ColorCV::nPorts(QtNodes::PortType portType) const {
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

QtNodes::NodeDataType ColorCV::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    return ImageData().type();
}

void ColorCV::setInData(std::shared_ptr<QtNodes::NodeData> const nodeData, const QtNodes::PortIndex portIndex) {
    m_inImageData = std::dynamic_pointer_cast<ImageData>(nodeData);
    const auto lockData = m_inImageData.lock();
    if (lockData) {
        const auto image = lockData->image();
        // Do something with the data
        if (!image.isNull()) {
            // Do something with the data
            const auto mat = QImageToMat(image);
            cv::Mat out;
            cv::cvtColor(mat, out, cv::COLOR_BGR2GRAY);
            m_outImageData = std::make_shared<ImageData>(MatToQImage(out));
        }
    }
    else {
        m_outImageData.reset();
    }
    Q_EMIT dataUpdated(0);
}

std::shared_ptr<QtNodes::NodeData> ColorCV::outData(const QtNodes::PortIndex port) {
    return m_outImageData;
}

QWidget* ColorCV::embeddedWidget() {
    if (!m_comboBox) {
        m_comboBox = new QComboBox();
        m_comboBox->addItem("BGR to GRAY", 6);
        m_comboBox->addItem("GRAY to BGR", 8);
        m_comboBox->setMaximumSize(m_comboBox->sizeHint());

        connect (m_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                 [=](int){
                     Q_EMIT dataUpdated(0);
                 });
    }
    return m_comboBox;
}
