//
// Created by pablo on 2/25/24.
//

#ifndef CANNYMODEL_H
#define CANNYMODEL_H

#include <QtNodes/NodeDelegateModel>
#include <QFutureWatcher>

class PixmapData;

namespace Ui {
    class CannyForm;
}

class CannyModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    CannyModel();

    ~CannyModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private:
    static QPixmap processImage(const QPixmap& pixmap, double lowThreshold, double highThreshold,
                                int apertureSize, bool useL2Gradient);

    void runCanny();

private slots:
    void requestCompute();

    void processFinished();

private:
    QWidget* m_widget = nullptr;
    Ui::CannyForm* m_ui = nullptr;

    std::shared_ptr<PixmapData> m_inPixmapData;
    QPixmap* m_pixmap = nullptr;

    QFutureWatcher<QPixmap> m_watcher;
    bool m_needToCompute = false;
};


#endif //CANNYMODEL_H
