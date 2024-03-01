//
// Created by pablo on 3/1/24.
//

#include "CascadeClassifierModel.h"
#include "ui_CascadeClassifierForm.h"

CascadeClassifier::CascadeClassifier() {
}

CascadeClassifier::~CascadeClassifier() {
}

QString CascadeClassifier::caption() const {
    return QString("Cascade Classifier");
}

QString CascadeClassifier::name() const {
    return QString("Cascade Classifier");
}

unsigned CascadeClassifier::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 1;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType CascadeClassifier::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return FileData().type();
        case QtNodes::PortType::Out:
            return CascadeClassifierData().type();
        default:
            return FileData().type();
    }
}

void CascadeClassifier::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            m_inFileData = std::dynamic_pointer_cast<FileData>(nodeData);
            const auto lockData = m_inFileData.lock();
            if (lockData) {
                const auto path = lockData->fileName();
                m_outFileData = std::make_shared<CascadeClassifierData>(path);
                if (m_ui) {
                    const bool isLoaded = !m_outFileData->isEmpty();
                    m_ui->cb_loaded->setChecked(isLoaded);
                }
            }
            else {
                m_outFileData.reset();
                if (m_ui) {
                    m_ui->cb_loaded->setChecked(false);
                }
            }
            emit dataUpdated(0);
        }
        break;
        default:
            break;
    }
}

std::shared_ptr<QtNodes::NodeData> CascadeClassifier::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outFileData;
        default:
            return nullptr;
    }
}

QWidget* CascadeClassifier::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = std::make_unique<Ui::CascadeClassifierForm>();
        m_ui->setupUi(m_widget);
    }
    return m_widget;
}
