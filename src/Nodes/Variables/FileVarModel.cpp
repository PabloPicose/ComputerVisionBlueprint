//
// Created by pablo on 3/1/24.
//

#include "FileVarModel.h"
#include "ui_FileVarForm.h"
#include <QFileDialog>

FileVarModel::FileVarModel() {
}

FileVarModel::~FileVarModel() {
}

QString FileVarModel::caption() const {
    return "File";
}

QString FileVarModel::name() const {
    return "File";
}

unsigned FileVarModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 0;
        case QtNodes::PortType::Out:
            return 1;
        default:
            return 0;
    }
}

QtNodes::NodeDataType FileVarModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return FileData().type();
        case QtNodes::PortType::Out:
            return FileData().type();
        default:
            return FileData().type();
    }
}

void FileVarModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> FileVarModel::outData(const QtNodes::PortIndex port) {
    return m_outFileData;
}

QWidget* FileVarModel::embeddedWidget() {
    if (!m_widget) {
        m_widget = new QWidget();
        m_ui = std::make_unique<Ui::FileVarForm>();
        m_ui->setupUi(m_widget);
        connect(m_ui->tb_find, &QToolButton::clicked, this, &FileVarModel::onBrowseButtonClicked);
        connect(m_ui->le_path, &QLineEdit::textChanged, this, &FileVarModel::onTextChanged);
    }
    return m_widget;
}

QJsonObject FileVarModel::save() const {
    QJsonObject modelJson = NodeDelegateModel::save();
    modelJson["path"] = m_ui->le_path->text();
    return modelJson;
}

void FileVarModel::load(QJsonObject const& jsonObj) {
    const QJsonValue path = jsonObj["path"];
    if (!path.isUndefined()) {
        onTextChanged(path.toString());
    }
}

void FileVarModel::onBrowseButtonClicked() {
    // open file dialog from the home directory
    const QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"), QDir::homePath());
    if (!fileName.isEmpty()) {
        onTextChanged(fileName);
    }
}

void FileVarModel::onTextChanged(const QString& text) {
    QSignalBlocker blocker(m_ui->le_path);
    m_ui->le_path->setText(text);
    const QFileInfo file(text);
    const bool exists = file.exists();
    m_ui->cb_exist->setChecked(exists);
    m_ui->le_path->adjustSize();
    if (exists) {
        m_ui->le_name->setText(file.fileName());
        m_outFileData = std::make_shared<FileData>(text);
    } else {
        m_outFileData.reset();
    }
    emit dataUpdated(0);
}
