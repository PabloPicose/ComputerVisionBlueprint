//
// Created by pablo on 3/1/24.
//

#ifndef FILEDATA_H
#define FILEDATA_H

#include <QtNodes/NodeData>

#include <QTemporaryFile>

class FileData final : public QtNodes::NodeData {
public:
    FileData() {
        m_file.setAutoRemove(true);
    }

    explicit FileData(const QString &filename) {
        // if the file exists, load the content into the temp file
        if (QFile::exists(filename)) {
            m_file.open();
            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            m_file.write(file.readAll());
            file.close();
            m_file.close();
        }
    }

    void setData(const QString &data) {
        m_file.setAutoRemove(true);
        m_file.open();
        m_file.write(data.toUtf8());
        m_file.close();
    }

    void setData(const QByteArray &data) {
        m_file.open();
        m_file.write(data);
        m_file.close();
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"file", "File"};
    }

    bool exists() const {
        return m_file.exists();
    }

    QString fileName() const {
        return m_file.fileName();
    }

private:
    QTemporaryFile m_file;
};

#endif //FILEDATA_H
