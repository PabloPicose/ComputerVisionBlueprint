//
// Created by pablo on 3/1/24.
//

#ifndef FILEDATA_H
#define FILEDATA_H

#include <QtNodes/NodeData>

#include <QFile>

class FileData final : public QtNodes::NodeData {
public:
    FileData() = default;

    explicit FileData(const QString &filename) : m_file(filename) {
    }

    explicit FileData(const QFile& file) : m_file(file.fileName()) {
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
    QFile m_file;
};

#endif //FILEDATA_H
