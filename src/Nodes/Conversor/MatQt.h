//
// Created by pablo on 2/25/24.
//

#ifndef MATQT_H
#define MATQT_H

#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>

QImage MatToQImage(const cv::Mat& mat);

QPixmap MatToQPixmap(const cv::Mat& mat);

cv::Mat QImageToMat(const QImage& image);

cv::Mat QPixmapToMat(const QPixmap& pixmap);
#endif //MATQT_H
