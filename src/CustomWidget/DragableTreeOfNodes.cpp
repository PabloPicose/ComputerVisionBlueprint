//
// Created by pablo on 3/11/24.
//

#include "DragableTreeOfNodes.h"
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/DataFlowGraphModel>
#include <QMimeData>
#include <QDrag>
#include <QPainter>

DragableTreeOfNodes::DragableTreeOfNodes(QWidget* parent) : QTreeWidget(parent) {
    setDragEnabled(true);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

DragableTreeOfNodes::~DragableTreeOfNodes() = default;

void DragableTreeOfNodes::fillTreeWidget(const std::shared_ptr<QtNodes::NodeDelegateModelRegistry>& registers) {
    const auto mapRegister = registers->registeredModelsCategoryAssociation();
    // print the categories from map
    for (auto it = mapRegister.begin(); it != mapRegister.end(); ++it) {
        const QString group = it->second;
        const QString name = it->first;
        m_mapGroupNames[group].append(name);
    }
    // now sort the names
    for (auto it = m_mapGroupNames.begin(); it != m_mapGroupNames.end(); ++it) {
        it.value().sort();
    }
    for (auto it = m_mapGroupNames.begin(); it != m_mapGroupNames.end(); ++it) {
        QTreeWidgetItem* parent = new QTreeWidgetItem(this);
        parent->setText(0, it.key());
        for (auto name: it.value()) {
            auto child = new QTreeWidgetItem(parent);
            child->setText(1, name);
        }
    }

    // show all the items
    expandAll();
    // resize columns to contents
    resizeColumnToContents(0);
    resizeColumnToContents(1);
}

void DragableTreeOfNodes::startDrag(Qt::DropActions supportedActions) {
    // only the childs nodes with the name filled will be dragged
    if (!currentItem() || currentItem()->text(1).isEmpty()) {
        return;
    }
    const QString name = currentItem()->text(1);
    QMimeData* mimeData = new QMimeData;
    mimeData->setText(name);
    // create a drag object
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    // Create a pixmap to display the text being dragged
    QPixmap pixmap(100, 30); // Adjust size as needed
    pixmap.fill(Qt::white); // Background color
    QPainter painter(&pixmap);
    painter.setPen(Qt::black); // Text color
    painter.drawText(pixmap.rect(), Qt::AlignCenter, name); // Draw the text on the pixmap
    drag->setPixmap(pixmap);

    drag->exec(supportedActions);
}
