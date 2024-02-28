//
// Created by pablo on 2/26/24.
//

#include "FacialDetectorModel.h"

#include <opencv2/opencv.hpp>


FacialDetectorModel::FacialDetectorModel() {
    cv::CascadeClassifier faceCascade;
    cv::Mat image;
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(image, faces, 1.2, 5, 0, cv::Size(30, 30));

    cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0);
    cv::dilate(image, image, cv::Mat(), cv::Point(-1, -1), 2, 1, 1);
    cv::HoughLinesP(image, image, 1, CV_PI / 180, 50, 50, 10);
}

FacialDetectorModel::~FacialDetectorModel() {
}

QString FacialDetectorModel::caption() const {
}

QString FacialDetectorModel::name() const {
}

unsigned FacialDetectorModel::nPorts(QtNodes::PortType portType) const {
}

QtNodes::NodeDataType FacialDetectorModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
}

void FacialDetectorModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
}

std::shared_ptr<QtNodes::NodeData> FacialDetectorModel::outData(const QtNodes::PortIndex port) {
}

QWidget* FacialDetectorModel::embeddedWidget() {
}
