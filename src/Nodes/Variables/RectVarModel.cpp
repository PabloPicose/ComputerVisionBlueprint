//
// Created by pablo on 3/1/24.
//

#include "RectVarModel.h"

RectVarModel::RectVarModel() {
}

RectVarModel::~RectVarModel() {
}

QString RectVarModel::caption() const {
    return "Rects";
}

QString RectVarModel::name() const {
    return "Rects";
}

unsigned RectVarModel::nPorts(QtNodes::PortType portType) const {
    switch (portType) {
        case QtNodes::PortType::In:
            return 2;
        case QtNodes::PortType::Out:
            return m_outRectsDataList.size() + 1;
            return m_outRectsData ? m_outRectsData->rects().size() : 0;
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
                case 1:
                    return RectData().type();
                default:
                    return VariantData().type();
            }
        case QtNodes::PortType::Out:
            switch (portIndex) {
                case 0:
                    return RectsData().type();
                default:
                    return RectData().type();
            }
        default:
            return VariantData().type();
    }
}

void RectVarModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, const QtNodes::PortIndex portIndex) {
    switch (portIndex) {
        case 0: {
            // detect in the map which value is expired and update it inserting the new value
            for (auto it = m_inVRectsDataMap.begin(); it != m_inVRectsDataMap.end(); ++it) {
                if (it.value().expired()) {
                    m_inVRectsDataMap.insert(it.key(), std::dynamic_pointer_cast<RectsData>(nodeData));
                    break;
                }
            }
        }
        break;
        case 1: {
            // detect in the map which value is expired and update it
            for (auto it = m_inRectDataMap.begin(); it != m_inRectDataMap.end(); ++it) {
                if (it.value().expired()) {
                    const int key = it.key();
                    const auto lock = std::dynamic_pointer_cast<RectData>(nodeData);
                    if (lock) {
                        m_inRectDataMap.insert(key, lock);
                    }
                    break;
                }
            }
        }
        default:
            break;
    }
    updateRect();
}

std::shared_ptr<QtNodes::NodeData> RectVarModel::outData(const QtNodes::PortIndex port) {
    switch (port) {
        case 0:
            return m_outRectsData;
        default:
            return m_outRectsDataList.at(port - 1);
    }
}

QWidget* RectVarModel::embeddedWidget() {
    if (!m_widget) {
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
                    return "Rect";
            }
        case QtNodes::PortType::Out:
            switch (port_index) {
                case 0:
                    return "Rects";
                default:
                    return "Rect";
            }
        default:
            return NodeDelegateModel::portCaption(port, port_index);
    }
}


QtNodes::ConnectionPolicy RectVarModel::portConnectionPolicy(QtNodes::PortType port,
                                                             QtNodes::PortIndex port_index) const {
    return QtNodes::ConnectionPolicy::Many;
}

void RectVarModel::updateRect() {
    QList<QRect> rects;
    // take all the inVRectsData and inRectData and create a new m_outRectsData
    for (auto it = m_inVRectsDataMap.begin(); it != m_inVRectsDataMap.end(); ++it) {
        if (const auto lock = it.value().lock()) {
            rects.append(lock->rects());
        }
    }
    // take all the inRectData and append it to the list
    for (auto it = m_inRectDataMap.begin(); it != m_inRectDataMap.end(); ++it) {
        if (const auto lock = it.value().lock()) {
            rects.append(lock->rect());
        }
    }
    m_outRectsDataList.clear();
    m_outRectsData.reset();
    if (!rects.isEmpty()) {
        m_outRectsData = std::make_shared<RectsData>(rects);
        for (const auto& rect: rects) {
            m_outRectsDataList.append(std::make_shared<RectData>(rect));
        }
    }
    // debug the outRectsData count and the outRectsDataList count
    //qDebug() << "RectVarModel::updateRect outRectsData count: " << m_outRectsDataList.size();
    //qDebug() << "RectVarModel::updateRect outRectsDataList count: " << m_outRectsDataList.size();

    // for each out node, emit dataUpdated
    for (int i = 0; i < nPorts(QtNodes::PortType::Out); ++i) {
        emit dataUpdated(i);
    }
}

void RectVarModel::inputConnectionDeleted(QtNodes::ConnectionId const& connection_id) {
    //qDebug() << "RectVarModel::inputConnectionDeleted connection_id in: " << connection_id.inNodeId;
    //qDebug() << "RectVarModel::inputConnectionDeleted connection_id out: " << connection_id.outNodeId;
    if (connection_id.inPortIndex == 0) {
        m_inVRectsDataMap.remove(connection_id.outNodeId);
    } else if (connection_id.inPortIndex == 1) {
        m_inRectDataMap.remove(connection_id.outNodeId);
    }
}

void RectVarModel::inputConnectionCreated(QtNodes::ConnectionId const& connection_id) {
    //qDebug() << "RectVarModel::inputConnectionCreated IN nodeID: " << connection_id.inNodeId;
    //qDebug() << "RectVarModel::inputConnectionCreated IN port index: " << connection_id.inPortIndex;
    //qDebug() << "RectVarModel::inputConnectionCreated OUT nodeID: " << connection_id.outNodeId;
    //qDebug() << "RectVarModel::inputConnectionCreated OUT port index: " << connection_id.outPortIndex;
    //qDebug() << "----------";
    // insert the outNodeID in the map m_inRectsData
    // the inNodeId is the port of the current node, where the connection is created
    if (connection_id.inPortIndex == 0) {
        m_inVRectsDataMap.insert(connection_id.outNodeId, std::weak_ptr<RectsData>());
    } else if (connection_id.inPortIndex == 1) {
        m_inRectDataMap.insert(connection_id.outNodeId, std::weak_ptr<RectData>());
    }
}
