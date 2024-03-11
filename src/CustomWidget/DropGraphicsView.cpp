//
// Created by pablo on 3/11/24.
//

#include "DropGraphicsView.h"
#include <QtNodes/DataFlowGraphicsScene>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QCursor>

#include "UndoCommands.hpp"

DropGraphicsView::DropGraphicsView(QWidget* parent) : QtNodes::GraphicsView(parent) {
    setAcceptDrops(true);
}

void DropGraphicsView::setMapGroupNames(const QMap<QString, QStringList>& mapGroupNames) {
    m_mapGroupNames = mapGroupNames;
}

bool DropGraphicsView::isNameContained(const QString& name) const {
    for (auto it = m_mapGroupNames.begin(); it != m_mapGroupNames.end(); ++it) {
        if (it.value().contains(name)) {
            return true;
        }
    }
    return false;
}

DropGraphicsView::~DropGraphicsView() {
}

void DropGraphicsView::placeNodeInScene(const QString& name, const QPoint& mousePos) {
    const auto scenePos = mapToScene(mousePos);
    m_Nodesscene->undoStack().push(new QtNodes::CreateCommand(m_Nodesscene, name, scenePos));
}

void DropGraphicsView::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasText()) {
        const QString droppedText = event->mimeData()->text();
        if (isNameContained(droppedText)) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void DropGraphicsView::dropEvent(QDropEvent* event) {
    if (event->mimeData()->hasText()) {
        // check if the name is contained in the map
        const QString droppedText = event->mimeData()->text();
        if (isNameContained(droppedText)) {
            placeNodeInScene(droppedText, event->position().toPoint());
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    } else {
        event->ignore();
    }
}

void DropGraphicsView::dragMoveEvent(QDragMoveEvent* event) {
    if (event->mimeData()->hasText()) { // Or any other condition you want to check
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DropGraphicsView::dragLeaveEvent(QDragLeaveEvent* event) {
    event->accept();
}
