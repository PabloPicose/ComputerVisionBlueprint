//
// Created by pablo on 3/11/24.
//

#ifndef PYRUP_H
#define PYRUP_H


#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>
#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/VariantData.h"

class QSpinBox;

class PyrUp final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    PyrUp();

    ~PyrUp() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    bool portCaptionVisible(QtNodes::PortType, QtNodes::PortIndex) const override;

    QString portCaption(QtNodes::PortType, QtNodes::PortIndex) const override;

private:
    static QPair<quint64, QImage> processImage(const QImage& image, const QSize& size);

private slots:
    void requestProcess();

    void processFinished();

private:
    QWidget* m_widget = nullptr;
    QSpinBox* m_spinBox = nullptr;
    // in
    // 0
    std::weak_ptr<ImageData> m_inImageData;
    QImage m_lastImageToProcess;
    // 1
    std::weak_ptr<VariantData> m_inSizeData;
    // out
    std::shared_ptr<ImageData> m_outImageData;
    QFutureWatcher<QPair<quint64, QImage>> m_watcher;
    bool m_processing = false;
};


#endif //PYRUP_H
