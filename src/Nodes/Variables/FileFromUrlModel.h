//
// Created by pablo on 3/11/24.
//

#ifndef FILEFROMURL_H
#define FILEFROMURL_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>


#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>
#include "Nodes/Data/FileData.h"

namespace Ui {
    class FileFromUrlForm;
}

class FileFromUrlModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    FileFromUrlModel();

    ~FileFromUrlModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private slots:

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void onError(QNetworkReply::NetworkError code);

    void onRequestFinished();

    void onTextEdited(const QString& text);

    void onStartRequest();

private:
    QNetworkAccessManager* m_networkManager = nullptr;
    QWidget* m_widget = nullptr;
    QNetworkReply* m_reply = nullptr;
    QScopedPointer<Ui::FileFromUrlForm> m_ui;

    QUrl m_url;

    // out
    //1
    std::shared_ptr<FileData> m_outFileData;
};


#endif //FILEFROMURL_H
