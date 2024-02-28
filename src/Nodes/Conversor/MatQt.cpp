//
// Created by pablo on 2/25/24.
//

#include "MatQt.h"
QImage MatToQImage(const cv::Mat& mat) {
    switch (mat.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            const QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return image.copy();
        }

        // 8-bit, 3 channel
        case CV_8UC3: {
            const QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped().copy();
        }

        // 8-bit, 1 channel
        case CV_8UC1: {
            static QVector<QRgb> sColorTable;
            if (sColorTable.isEmpty()) {
                for (int i = 0; i < 256; ++i)
                    sColorTable.push_back(qRgb(i, i, i));
            }
            QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
            image.setColorTable(sColorTable);
            return image.copy();
        }

        default:
            qWarning("MatToQImage() - cv::Mat image type not handled in switch:");
            break;
    }

    return QImage();
}


cv::Mat QImageToMat(const QImage& image) {
    cv::Mat mat;
    switch (image.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                          const_cast<uchar *>(image.bits()), image.bytesPerLine());
            break;
        }
        case QImage::Format_RGB32: {
            mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                          const_cast<uchar *>(image.bits()), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
            break;
        }
        case QImage::Format_RGB888: {
            QImage swapped = image.rgbSwapped();
            mat = cv::Mat(swapped.height(), swapped.width(), CV_8UC3,
                          const_cast<uchar *>(swapped.bits()), swapped.bytesPerLine());
            break;
        }
        case QImage::Format_Grayscale8: {
            mat = cv::Mat(image.height(), image.width(), CV_8UC1,
                          const_cast<uchar *>(image.bits()), image.bytesPerLine());
            break;
        }
        // Add other QImage formats if needed.
        default: {
            // Unsupported format
            break;
        }
    }
    return mat;
}
