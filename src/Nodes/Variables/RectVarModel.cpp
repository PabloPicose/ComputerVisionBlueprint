//
// Created by pablo on 3/1/24.
//

#include "RectVarModel.h"
#include "ui_RectVarForm.h"

RectVarModel::RectVarModel() {
}

RectVarModel::~RectVarModel() {
}

QString RectVarModel::caption() const {
    return "Rect";
}

QString RectVarModel::name() const {
    return "Rect";
}

unsigned RectVarModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 1;
        case QtNodes::PortType::Out:
            return 5;
        default:
            return 0;
    }
}

QtNodes::NodeDataType RectVarModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {
    switch (portType) {
        case QtNodes::PortType::In:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                default:
                    return VariantData().type();
            }
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                case 1:
                    return PointData().type();
                case 2:
                    return PointData().type();
                case 3:
                    return VariantData(0).type();
                case 4:
                    return VariantData(0).type();
                case 5:
                    return PointData().type();
                default:
                    return VariantData().type();
            }
        default:
            return VariantData().type();
    }
}

void RectVarModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0:
            m_inRectsData = std::dynamic_pointer_cast<RectsData>(nodeData);
            if (m_inRectsData.expired()) {
                if (m_ui) {
                    m_ui->sb_x->setEnabled(true);
                    m_ui->sb_y->setEnabled(true);
                    m_ui->sb_width->setEnabled(true);
                    m_ui->sb_height->setEnabled(true);
                }
            } else {
                if (m_ui) {
                    m_ui->sb_x->setEnabled(false);
                    m_ui->sb_y->setEnabled(false);
                    m_ui->sb_width->setEnabled(false);
                    m_ui->sb_height->setEnabled(false);
                }
            }
            break;
        default:
            break;
    }
    updateRect();
}

std::shared_ptr<QtNodes::NodeData> RectVarModel::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outRectsData;
        case 1:
            return m_outCenterPointData;
        case 2:
            return m_outTopLeftPointData;
        case 3:
            return m_outWidth;
        case 4:
            return m_outHeight;
        case 5:
            return m_outCenter;
        default:
            return nullptr;
    }
}

QWidget* RectVarModel::embeddedWidget() {
    if (!m_widget) {
        m_ui = std::make_unique<Ui::RectVarForm>();
        m_widget = new QWidget();
        m_ui->setupUi(m_widget);
        // widgets are QSpinBoxes:
        // top left widgets: sb_x and sb_y
        // size widgets: sb_width and sb_height
        // center widgets: sb_center_x and sb_center_y only read
        connect(m_ui->sb_x, qOverload<int>(&QSpinBox::valueChanged), this, &RectVarModel::updateRect);
        connect(m_ui->sb_y, qOverload<int>(&QSpinBox::valueChanged), this, &RectVarModel::updateRect);
        connect(m_ui->sb_width, qOverload<int>(&QSpinBox::valueChanged), this, &RectVarModel::updateRect);
        connect(m_ui->sb_height, qOverload<int>(&QSpinBox::valueChanged), this, &RectVarModel::updateRect);

    }
    return m_widget;
}

QString RectVarModel::portCaption(QtNodes::PortType port, QtNodes::PortIndex port_index) const {
    switch (port) {
        case QtNodes::PortType::In:
            switch (port_index) {
                case 0:
                    return "Rects";
                default:
                    return "Unknown";
            }
        case QtNodes::PortType::Out:
            switch (port_index) {
                case 0:
                    return "Rects";
                case 1:
                    return "Center Point";
                case 2:
                    return "Top Left Point";
                case 3:
                    return "Width";
                case 4:
                    return "Height";
                case 5:
                    return "Center";
                default:
                    return "Unknown";
            }
        default:
            return NodeDelegateModel::portCaption(port, port_index);
    }
}

void RectVarModel::updateRect() {
    // update the m_rect, if inRectsData is expired take the values from the m_ui
    const auto lockRectsData = m_inRectsData.lock();
    if (lockRectsData) {
        const auto rects = lockRectsData->rects();
        if (rects.isEmpty()) {
            return;
        }
        m_rect = rects.at(0);
        // update the m_ui blocking the signals
        if (m_ui) {
            QSignalBlocker blockerX(m_ui->sb_x);
            QSignalBlocker blockerY(m_ui->sb_y);
            QSignalBlocker blockerWidth(m_ui->sb_width);
            QSignalBlocker blockerHeight(m_ui->sb_height);
            m_ui->sb_x->setValue(m_rect.x());
            m_ui->sb_y->setValue(m_rect.y());
            m_ui->sb_width->setValue(m_rect.width());
            m_ui->sb_height->setValue(m_rect.height());
            m_ui->sb_center_x->setValue(m_rect.center().x());
            m_ui->sb_center_y->setValue(m_rect.center().y());
        }
    } else if (m_ui) {
        m_rect.setX(m_ui->sb_x->value());
        m_rect.setY(m_ui->sb_y->value());
        m_rect.setWidth(m_ui->sb_width->value());
        m_rect.setHeight(m_ui->sb_height->value());
    }
    // update the out data
    m_outRectsData = std::make_shared<RectsData>(QList{m_rect});
    m_outCenterPointData = std::make_shared<PointData>(m_rect.center());
    m_outTopLeftPointData = std::make_shared<PointData>(m_rect.topLeft());
    m_outWidth = std::make_shared<VariantData>(m_rect.width());
    m_outHeight = std::make_shared<VariantData>(m_rect.height());
    m_outCenter = std::make_shared<PointData>(m_rect.center());
    emit dataUpdated(0);
    emit dataUpdated(1);
    emit dataUpdated(2);
    emit dataUpdated(3);
    emit dataUpdated(4);
    emit dataUpdated(5);
}
