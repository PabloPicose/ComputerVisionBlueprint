#include "ImageFromFile.h"

ImageFromFile::ImageFromFile()
{
}

unsigned int ImageFromFile::nPorts(QtNodes::PortType portType) const
{
    // one for in, one for out
    return 1;
}

QtNodes::NodeDataType ImageFromFile::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    switch (portType) {
    case QtNodes::PortType::In:
        return FileData().type();
    case QtNodes::PortType::Out:
        return ImageData().type();
    }
    return QtNodes::NodeDataType();
}

std::shared_ptr<QtNodes::NodeData> ImageFromFile::outData(QtNodes::PortIndex port)
{
    return m_outImageData;
}

void ImageFromFile::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex)
{
    switch (portIndex) {
    case 0:
        m_inFileData = std::dynamic_pointer_cast<FileData>(nodeData);
        loadImage();
        break;
    }
}

QWidget* ImageFromFile::embeddedWidget()
{
    return nullptr;
}

void ImageFromFile::loadImage()
{
    const auto file_lock = m_inFileData.lock();
    if (!file_lock) {
        m_outImageData.reset();
        return;
    } else {
        m_outImageData = std::make_shared<ImageData>(file_lock->fileName());
    }
    emit dataUpdated(0);
}
