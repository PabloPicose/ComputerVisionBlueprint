//
// Created by pablo on 2/24/24.
//

#ifndef BLURMODEL_H
#define BLURMODEL_H


#include <QtNodes/NodeDelegateModel>
#include <QReadWriteLock>
#include <QFutureWatcher>


class SizeData;
class PixmapData;

namespace Ui {
    class BlurForm;
}

class BlurModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    BlurModel();

    ~BlurModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    static QPixmap processImage(const QSize& size, const QPixmap& pixmap);

    QPixmap getPixmapToProcess();

private slots:
    void processFinished();

    void requestProcess();

private:
    QWidget* m_widget = nullptr;
    Ui::BlurForm* m_ui = nullptr;

    std::weak_ptr<PixmapData> m_inPixmapData;
    std::shared_ptr<PixmapData> m_outPixmapData;

    QPixmap m_lastPixmapToProcess;

    QFutureWatcher<QPixmap> m_watcher;
};


#endif //BLURMODEL_H
