//
// Created by pablo on 3/11/24.
//

#ifndef DROPGRAPHICSVIEW_H
#define DROPGRAPHICSVIEW_H

#include <QtNodes/GraphicsView>

namespace QtNodes {
    class DataFlowGraphicsScene;
}

class DropGraphicsView final : public QtNodes::GraphicsView {
    Q_OBJECT

public:
    explicit DropGraphicsView(QWidget* parent = nullptr);

    void setDataFlowScene(QtNodes::DataFlowGraphicsScene* scene) { m_Nodesscene = scene; }

    void setMapGroupNames(const QMap<QString, QStringList>& mapGroupNames);

    bool isNameContained(const QString& name) const;

    ~DropGraphicsView() override;

    void placeNodeInScene(const QString& name, const QPoint& mousePos);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;

    void dropEvent(QDropEvent* event) override;

    void dragMoveEvent(QDragMoveEvent* event) override;

    void dragLeaveEvent(QDragLeaveEvent* event) override;

private:
    QtNodes::DataFlowGraphicsScene* m_Nodesscene = nullptr;

    QMap<QString, QStringList> m_mapGroupNames;
};


#endif //DROPGRAPHICSVIEW_H
