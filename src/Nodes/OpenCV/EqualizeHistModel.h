//
// Created by pablo on 3/1/24.
//

#ifndef EQUIALIZEHIST_H
#define EQUIALIZEHIST_H


#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/ImageData.h"

#include <QFutureWatcher>

class QLabel;

class EqualizeHistModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    EqualizeHistModel();

    ~EqualizeHistModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override;

private slots:
    void processFinished();

    void requestProcess();
private:
    static QPair<QImage, quint64> equializeHist(const QImage& image);

private:
    QLabel* m_widget = nullptr;
    QImage m_lastImageToProcess;
    std::weak_ptr<ImageData> m_inImageData;

    std::shared_ptr<ImageData> m_outImageData;

    QFutureWatcher<QPair<QImage, quint64>> m_watcher;
};


#endif //EQUIALIZEHIST_H
