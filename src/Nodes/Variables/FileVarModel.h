//
// Created by pablo on 3/1/24.
//

#ifndef FILEVARMODEL_H
#define FILEVARMODEL_H

#include <QtNodes/NodeDelegateModel>
#include "Nodes/Data/FileData.h"

namespace Ui {
    class FileVarForm;
}


class FileVarModel final : public QtNodes::NodeDelegateModel {
    Q_OBJECT

public:
    FileVarModel();

    ~FileVarModel() override;

    QString caption() const override;

    QString name() const override;

    unsigned nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) override;

    std::shared_ptr<QtNodes::NodeData> outData(const QtNodes::PortIndex port) override;

    QWidget* embeddedWidget() override;

private slots:
    void onBrowseButtonClicked();

    void onTextChanged(const QString& text);

private:
    std::unique_ptr<Ui::FileVarForm> m_ui;
    QWidget* m_widget = nullptr;

    // out
    std::shared_ptr<FileData> m_outFileData;
};


#endif //FILEVARMODEL_H
