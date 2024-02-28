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

        explicit CascadeClassifierData(const cv::CascadeClassifier& cascadeClassifier) : m_cascadeClassifier(cascadeClassifier) {
        }

        QtNodes::NodeDataType type() const override {
            return QtNodes::NodeDataType{"cascadeClassifier", "Cascade Classifier"};
        }

        cv::CascadeClassifier cascadeClassifier() const {
            return m_cascadeClassifier;
        }

        void setCascadeClassifier(const cv::CascadeClassifier& cascadeClassifier) {
            m_cascadeClassifier = cascadeClassifier;
        }
private:
    cv::CascadeClassifier m_cascadeClassifier;
};

#endif //CASCADECLASSIFIERDATA_H
