//
// Created by pablo on 3/11/24.
//

#include "FileFromUrlModel.h"
#include "ui_FileFromUrlForm.h"

FileFromUrlModel::FileFromUrlModel() {
    m_networkManager = new QNetworkAccessManager(this);
}

FileFromUrlModel::~FileFromUrlModel() {
}

QString FileFromUrlModel::caption() const {
    return QString("File From URL");
}

QString FileFromUrlModel::name() const {
    return QString("File From URL");
}

unsigned FileFromUrlModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 0;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType FileFromUrlModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::Out:
            return FileData().type();
        default:
            return FileData().type();
    }
}

void FileFromUrlModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> FileFromUrlModel::outData(const QtNodes::PortIndex port) {
    return m_outFileData;
}

QWidget* FileFromUrlModel::embeddedWidget() {
    if (m_widget == nullptr) {
        m_widget = new QWidget();
        m_ui.reset(new Ui::FileFromUrlForm);
        m_ui->setupUi(m_widget);
        connect(m_ui->le_url, &QLineEdit::textEdited, this, &FileFromUrlModel::onTextEdited);
        connect(m_ui->pb_request, &QPushButton::clicked, this, &FileFromUrlModel::onStartRequest);
    }
    return m_widget;
}

void FileFromUrlModel::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    // update the progressBar
    if (m_ui) {
        m_ui->progressBar->setMaximum(bytesTotal);
        m_ui->progressBar->setValue(bytesReceived);
    }
}

void FileFromUrlModel::onError(QNetworkReply::NetworkError code) {
    m_reply->deleteLater();
    if (m_ui) {
        m_ui->pb_request->setEnabled(true);
        m_ui->cb_error->setChecked(true);
        // reset the progressBar
        m_ui->progressBar->setValue(0);
    }
}

void FileFromUrlModel::onRequestFinished() {
    m_reply->deleteLater();
    if (m_ui) {
        m_ui->pb_request->setEnabled(true);
    }
    if (m_reply->error() == QNetworkReply::NoError) {
        // create a FileData with the downloaded file
        m_outFileData = std::make_shared<FileData>();
        m_outFileData->setData(m_reply->readAll());
        if (m_ui) {
            m_ui->cb_loaded->setChecked(true);
        }
    } else {
        m_outFileData.reset();
        m_ui->cb_error->setChecked(true);
    }

    emit dataUpdated(0);
}

void FileFromUrlModel::onTextEdited(const QString& text) {
    m_url = QUrl(text);
    if (m_ui) {
        m_ui->pb_request->setEnabled(m_url.isValid());
    }
}

void FileFromUrlModel::onStartRequest() {
    if (m_url.isValid()) {
        if (m_ui) {
            m_ui->pb_request->setEnabled(false);
            m_ui->progressBar->setValue(0);
            m_ui->cb_empty->setChecked(true);
        }
        // start the request
        const QNetworkRequest request(m_url);
        m_reply = m_networkManager->get(request);
        connect(m_reply, &QNetworkReply::downloadProgress, this, &FileFromUrlModel::onDownloadProgress);
        connect(m_reply, &QNetworkReply::errorOccurred, this, &FileFromUrlModel::onError);
        connect(m_reply, &QNetworkReply::finished, this, &FileFromUrlModel::onRequestFinished);
    }
}
