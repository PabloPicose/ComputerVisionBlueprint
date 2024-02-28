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
}

QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow() override;

private:
    static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();

private:
    Ui::MainWindow* ui;
    QtNodes::DataFlowGraphicsScene* m_scene = nullptr;
};


#endif //MAINWINDOW_H
