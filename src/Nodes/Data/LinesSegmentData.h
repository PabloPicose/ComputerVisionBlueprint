//
// Created by pablo on 2/27/24.
//

#ifndef LINESSEGMENT_H
#define LINESSEGMENT_H

#include <QtNodes/NodeData>
#include <QVector>
#include <QLine>

#include <opencv2/opencv.hpp>

typedef QVector<QLine> LinesSegment;

class LinesSegmentData final : public QtNodes::NodeData {
public:
    LinesSegmentData() {
    }

    explicit LinesSegmentData(const LinesSegment& lines) : m_lines(lines) {
    }

    explicit LinesSegmentData(const std::vector<cv::Vec4i>& lines) {
        for (const auto& line : lines) {
            m_lines.push_back(QLine(line[0], line[1], line[2], line[3]));
        }
    }

    std::vector<cv::Vec4i> linesCV() const {
        std::vector<cv::Vec4i> lines;
        for (const auto& line : m_lines) {
            lines.push_back(cv::Vec4i(line.x1(), line.y1(), line.x2(), line.y2()));
        }
        return lines;
    }

    LinesSegment lines() {
        return m_lines;
    }

    LinesSegment lines() const {
        return m_lines;
    }

    QtNodes::NodeDataType type() const override {
        return QtNodes::NodeDataType{"linessegment", "Lines Segment"};
    }

private:
    LinesSegment m_lines;
};


#endif //LINESSEGMENT_H
