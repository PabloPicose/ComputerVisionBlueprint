//
// Created by pablo on 2/24/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/DataFlowGraphModel>
#include "Nodes/NodesInclude.h"

#include <QtNodes/GraphicsView>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_model = new QtNodes::DataFlowGraphModel(registerDataModels());
    m_scene = new QtNodes::DataFlowGraphicsScene(*m_model);
    m_model->setParent(m_scene);

    ui->nodes_graphicsView->setScene(m_scene);

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSaveTriggered);
    connect(ui->actionLoad, &QAction::triggered, this, &MainWindow::onActionLoadTriggered);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onActionSaveTriggered() {
    m_scene->save();
}

void MainWindow::onActionLoadTriggered() {
    m_scene->load();
}

std::shared_ptr<QtNodes::NodeDelegateModelRegistry> MainWindow::registerDataModels() {
        auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    ret->registerModel<PiModel>("Constants");

    ret->registerModel<NumberSourceDataModel>("Sources");

    ret->registerModel<NumberDisplayDataModel>("Displays");

    ret->registerModel<AdditionModel>("Operators");
    ret->registerModel<SubtractionModel>("Operators");
    ret->registerModel<MultiplicationModel>("Operators");
    ret->registerModel<DivisionModel>("Operators");

    ret->registerModel<ImageLoaderModel>("Images");
    ret->registerModel<ImageShowModel>("Images");
    ret->registerModel<ImageInfoModel>("Images");
    ret->registerModel<DrawLinesModel>("Images");
    ret->registerModel<DrawRectsModel>("Images");
    ret->registerModel<ConvertImageToModel>("Images");
    ret->registerModel<ScaleImageModel>("Images");
    ret->registerModel<CutImageModel>("Images");

    ret->registerModel<ColorCVModel>("OpenCV");
    ret->registerModel<BlurModel>("OpenCV");
    ret->registerModel<CannyModel>("OpenCV");
    ret->registerModel<GaussianBlurModel>("OpenCV");
    ret->registerModel<HoughLinesPModel>("OpenCV");
    ret->registerModel<EqualizeHistModel>("OpenCV");

    ret->registerModel<CascadeClassifier>("CascadeClassifier");
    ret->registerModel<DetectMultiScaleModel>("CascadeClassifier");

    ret->registerModel<SizeVarModel>("Variables");
    ret->registerModel<FileVarModel>("Variables");
    ret->registerModel<RectVarModel>("Variables");
    ret->registerModel<RectModel>("Variables");

    ret->registerModel<ScaleRects>("Data Operations");

    return ret;
}
