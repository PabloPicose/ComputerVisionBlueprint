//
// Created by pablo on 2/24/24.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace QtNodes {
    class DataFlowGraphicsScene;
    class DataFlowGraphModel;
    class NodeDelegateModelRegistry;
    class UndoStack;
}

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

protected:
    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    void onActionSaveTriggered();

    void onActionLoadTriggered();

private:
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();

private:
    Ui::MainWindow* ui;
    QtNodes::DataFlowGraphModel* m_model = nullptr;
    QtNodes::DataFlowGraphicsScene* m_scene = nullptr;

    QMap<QString, QStringList> m_mapGroupNames;
};


#endif //MAINWINDOW_H
