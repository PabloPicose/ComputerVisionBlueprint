//
// Created by pablo on 3/11/24.
//

#ifndef DRAGABLETREEOFNODES_H
#define DRAGABLETREEOFNODES_H

#include <QTreeWidget>

namespace QtNodes {
    class NodeDelegateModelRegistry;
}

class DragableTreeOfNodes final : public QTreeWidget {
    Q_OBJECT

public:
    explicit DragableTreeOfNodes(QWidget* parent = nullptr);

    ~DragableTreeOfNodes() override;

    void fillTreeWidget(const std::shared_ptr<QtNodes::NodeDelegateModelRegistry>& registers);

    QMap<QString, QStringList> getMapGroupNames() const { return m_mapGroupNames; }

protected:
    void startDrag(Qt::DropActions supportedActions) override;
private:
    QMap<QString, QStringList> m_mapGroupNames;
};


#endif //DRAGABLETREEOFNODES_H
