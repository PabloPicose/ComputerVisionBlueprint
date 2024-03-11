cmake_minimum_required(VERSION 3.28)
project(OpenCVNodeEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets Gui Concurrent Multimedia REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#find opencv
find_package(OpenCV CONFIG REQUIRED)

add_subdirectory(3rdparty/nodeeditor)

add_executable(ComputerVisionBlueprint
        resources/images/images.qrc
        src/main.cpp
        #src/Nodes/OpenCV/FacialDetectorModel.cpp
        #src/Nodes/OpenCV/FacialDetectorModel.h
        #src/Nodes/OpenCV/RectangleForm.ui
        #src/Nodes/OpenCV/RectangleModel.cpp
        #src/Nodes/OpenCV/RectangleModel.h
        src/CustomWidget/CheckBoxReadOnly.cpp
        src/CustomWidget/CheckBoxReadOnly.h
        src/Nodes/Basic/AdditionModel.h
        src/Nodes/Basic/DivisionModel.h
        src/Nodes/Basic/MathOperationDataModel.cpp
        src/Nodes/Basic/MathOperationDataModel.hpp
        src/Nodes/Basic/MultiplicationModel.hpp
        src/Nodes/Basic/NumberDisplayDataModel.cpp
        src/Nodes/Basic/NumberDisplayDataModel.hpp
        src/Nodes/Basic/NumberSourceDataModel.cpp
        src/Nodes/Basic/NumberSourceDataModel.hpp
        src/Nodes/Basic/SubtractionModel.h
        src/Nodes/Constants/PiModel.cpp
        src/Nodes/Constants/PiModel.h
        src/Nodes/Conversor/MatQt.cpp
        src/Nodes/Conversor/MatQt.h
        src/Nodes/Data/CascadeClassifierData.h
        src/Nodes/Data/DataInclude.h
        src/Nodes/Data/FileData.h
        src/Nodes/Data/ImageData.h
        src/Nodes/Data/ImageFormatData.h
        src/Nodes/Data/LinesSegmentData.h
        src/Nodes/Data/RectsData.h
        src/Nodes/Data/VariantData.h
        src/Nodes/Images/DrawLinesForm.ui
        src/Nodes/Images/DrawLinesModel.cpp
        src/Nodes/Images/DrawLinesModel.h
        src/Nodes/Images/DrawRectsForm.ui
        src/Nodes/Images/DrawRectsModel.cpp
        src/Nodes/Images/DrawRectsModel.h
        src/Nodes/Images/ImageInfoForm.ui
        src/Nodes/Images/ImageInfoModel.cpp
        src/Nodes/Images/ImageInfoModel.h
        src/Nodes/Images/ImageLoaderModel.cpp
        src/Nodes/Images/ImageLoaderModel.hpp
        src/Nodes/Images/ImageShowModel.cpp
        src/Nodes/Images/ImageShowModel.hpp
        src/Nodes/NodesInclude.cpp
        src/Nodes/NodesInclude.h
        src/Nodes/OpenCV/BlurForm.ui
        src/Nodes/OpenCV/BlurModel.cpp
        src/Nodes/OpenCV/BlurModel.h
        src/Nodes/OpenCV/CannyForm.ui
        src/Nodes/OpenCV/CannyModel.cpp
        src/Nodes/OpenCV/CannyModel.h
        src/Nodes/OpenCV/CascadeClassifierForm.ui
        src/Nodes/OpenCV/CascadeClassifierModel.cpp
        src/Nodes/OpenCV/CascadeClassifierModel.h
        src/Nodes/OpenCV/ColorCVModel.cpp
        src/Nodes/OpenCV/ColorCVModel.h
        src/Nodes/OpenCV/DetectMultiScaleModel.cpp
        src/Nodes/OpenCV/DetectMultiScaleModel.h
        src/Nodes/OpenCV/EqualizeHistModel.cpp
        src/Nodes/OpenCV/EqualizeHistModel.h
        src/Nodes/OpenCV/GaussianBlurForm.ui
        src/Nodes/OpenCV/GaussianBlurModel.cpp
        src/Nodes/OpenCV/GaussianBlurModel.h
        src/Nodes/OpenCV/HoughLinesPModel.cpp
        src/Nodes/OpenCV/HoughLinesPModel.h
        src/Nodes/OpenCV/HoughLinesPForm.ui
        src/Nodes/Variables/FileVarForm.ui
        src/Nodes/Variables/FileVarModel.cpp
        src/Nodes/Variables/FileVarModel.h
        src/Nodes/Variables/SizeVarForm.ui
        src/Nodes/Variables/SizeVarModel.cpp
        src/Nodes/Variables/SizeVarModel.h
        src/Widgets/MainWindow.cpp
        src/Widgets/MainWindow.h
        src/Widgets/MainWindow.ui
        src/Nodes/Variables/RectVarModel.cpp
        src/Nodes/Variables/RectVarModel.h
        src/Nodes/Data/PointData.h
        src/Nodes/Data/RectData.h
        src/Nodes/Images/ConvertImageToModel.cpp
        src/Nodes/Images/ConvertImageToModel.h
        src/Nodes/Images/ConvertImageToForm.ui
        src/Nodes/Images/ScaleImageModel.cpp
        src/Nodes/Images/ScaleImageModel.h
        src/Nodes/Images/ScaleImageForm.ui
        src/Nodes/DataOperations/ScaleRects.cpp
        src/Nodes/DataOperations/ScaleRects.h
        src/Nodes/Variables/RectModel.cpp
        src/Nodes/Variables/RectModel.h
        src/Nodes/Variables/RectForm.ui
        src/Nodes/Images/CutImageModel.cpp
        src/Nodes/Images/CutImageModel.h
        src/Nodes/Video/CameraModel.cpp
        src/Nodes/Video/CameraModel.h
        src/Nodes/Video/CameraForm.ui
        src/Nodes/Video/CaptureModel.cpp
        src/Nodes/Video/CaptureModel.h
        src/CustomWidget/DragableTreeOfNodes.cpp
        src/CustomWidget/DragableTreeOfNodes.h
        src/CustomWidget/DropGraphicsView.cpp
        src/CustomWidget/DropGraphicsView.h
        src/Nodes/OpenCV/PyrDown.cpp
        src/Nodes/OpenCV/PyrDown.h
)

target_link_libraries(ComputerVisionBlueprint PRIVATE
        QtNodes::QtNodes
        ${OpenCV_LIBS}
        Qt6::Concurrent
        Qt6::Multimedia
)

target_include_directories(ComputerVisionBlueprint PRIVATE src)
target_include_directories(ComputerVisionBlueprint PRIVATE 3rdparty/nodeeditor/src)
target_include_directories(ComputerVisionBlueprint PRIVATE 3rdparty/nodeeditor/include/QtNodes/internal)
