#include <QApplication>

#include <QtNodes/DataFlowGraphicsScene>
#include "Widgets/MainWindow.h"

static void setStyle() {
    QtNodes::ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logo.png"));
    setStyle();

    MainWindow w;
    w.show();

    return a.exec();
}
