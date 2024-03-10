//
// Created by pablo on 3/2/24.
//

#ifndef CONVERTIMAGETOMODEL_H
#define CONVERTIMAGETOMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

#include "Nodes/Data/ImageData.h"
#include "Nodes/Data/ImageFormatData.h"

class QComboBox;

namespace Ui {
    class ConvertImageToForm;
}

class ConvertImageToModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    ConvertImageToModel();

    ~ConvertImageToModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

    QJsonObject save() const override;

    void load(QJsonObject const& jsonObj) override;

private:
    static QPair<QImage, quint64> processImage(const QImage& image, QImage::Format format,
                                               QFlags<Qt::ImageConversionFlag> flags);

    void fillComboBoxFormats(QComboBox* comboBox);

    QImage::Format getFormat() const;

private slots:
    void processFinished();

    void requestProcess();

    QImage getPixmapToProcess() const;

    void updateFlags();

    void updateFormat(int index);

    void flagsClicked();

private:
    QWidget* m_widget = nullptr;
    std::unique_ptr<Ui::ConvertImageToForm> m_ui;
    QFutureWatcher<QPair<QImage, quint64>> m_watcher;
    QFlags<Qt::ImageConversionFlag> m_flags;
    bool m_processing = false;
    // in
    // 0
    QImage m_lastImageToProcess;
    std::weak_ptr<ImageData> m_inImageData;
    // 1
    QImage::Format m_lastFormatToProcess;
    std::weak_ptr<ImageFormatData> m_inImageFormatData;
    // out
    //0
    std::shared_ptr<ImageData> m_outImageData;
    //1
    QImage::Format m_outImageFormat;
    std::shared_ptr<ImageFormatData> m_outImageFormatData;
};


#endif //CONVERTIMAGETOMODEL_H
