//
// Created by pablo on 2/26/24.
//

#ifndef CASCADECLASSIFIERDATA_H
#define CASCADECLASSIFIERDATA_H

#include <QtNodes/NodeData>
#include <opencv2/opencv.hpp>

class CascadeClassifierData final : public QtNodes::NodeData {
public:
    CascadeClassifierData() {
    }

    explicit CascadeClassifierData(const QString& filename) : m_cascadeClassifier(
        std::make_shared<cv::CascadeClassifier>()) {
        m_cascadeClassifier->load(filename.toStdString());
    }

    explicit CascadeClassifierData(const cv::CascadeClassifier& cascadeClassifier) : m_cascadeClassifier(
        std::make_shared<cv::CascadeClassifier>(cascadeClassifier)) {
    }


    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"cascadeClassifier", "Cascade Classifier"};
    }

    std::shared_ptr<cv::CascadeClassifier> cascadeClassifier() const {
        return m_cascadeClassifier;
    }

    bool isEmpty() const {
        if (m_cascadeClassifier) {
            return m_cascadeClassifier->empty();
        }
        return true;
    }

private:
    std::shared_ptr<cv::CascadeClassifier> m_cascadeClassifier;
};

#endif //CASCADECLASSIFIERDATA_H
