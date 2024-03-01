cmake_minimum_required(VERSION 3.28)
project(OpenCVNodeEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS Widgets Gui Concurrent REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#find opencv
find_package(OpenCV REQUIRED QUIET)

add_subdirectory(3rdparty/nodeeditor)

add_executable(OpenCVNodeEditor
        src/main.cpp
        # nodes basic
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
        # nodes images
        src/Nodes/Images/ImageLoaderModel.cpp
        src/Nodes/Images/ImageLoaderModel.hpp
        src/Nodes/Images/ImageShowModel.cpp
        src/Nodes/Images/ImageShowModel.hpp
        # nodes data
        src/Nodes/Data/ImageData.h

        # nodes include
        src/Nodes/NodesInclude.h

        # widgets
        src/Widgets/MainWindow.cpp
        src/Widgets/MainWindow.h
        src/Widgets/MainWindow.ui
        src/Nodes/NodesInclude.cpp
        src/Nodes/OpenCV/ColorCVModel.cpp
        src/Nodes/OpenCV/ColorCVModel.h
        src/Nodes/OpenCV/BlurModel.cpp
        src/Nodes/OpenCV/BlurModel.h
        src/Nodes/OpenCV/BlurForm.ui

        #src/Nodes/OpenCV/FacialDetectorModel.cpp
        #src/Nodes/OpenCV/FacialDetectorModel.h
        #src/Nodes/OpenCV/RectangleForm.ui
        #src/Nodes/OpenCV/RectangleModel.cpp
        #src/Nodes/OpenCV/RectangleModel.h
        src/CustomWidget/CheckBoxReadOnly.cpp
        src/CustomWidget/CheckBoxReadOnly.h
        src/Nodes/Constants/PiModel.cpp
        src/Nodes/Constants/PiModel.h
        src/Nodes/Conversor/MatQt.cpp
        src/Nodes/Conversor/MatQt.h
        src/Nodes/Data/CascadeClassifierData.h
        src/Nodes/Data/DataInclude.h
        src/Nodes/Data/ImageFormatData.h
        src/Nodes/Data/LinesSegmentData.h
        src/Nodes/Data/VariantData.h
        src/Nodes/Images/ImageInfoForm.ui
        src/Nodes/Images/ImageInfoModel.cpp
        src/Nodes/Images/ImageInfoModel.h
        src/Nodes/OpenCV/CannyForm.ui
        src/Nodes/OpenCV/CannyModel.cpp
        src/Nodes/OpenCV/CannyModel.h
        src/Nodes/OpenCV/GaussianBlurForm.ui
        src/Nodes/OpenCV/GaussianBlurModel.cpp
        src/Nodes/OpenCV/GaussianBlurModel.h
        src/Nodes/OpenCV/HoughLinesP.cpp
        src/Nodes/OpenCV/HoughLinesP.h
        src/Nodes/OpenCV/HoughLinesPForm.ui
        src/Nodes/Variables/SizeVarForm.ui
        src/Nodes/Variables/SizeVarModel.cpp
        src/Nodes/Variables/SizeVarModel.h
        src/Nodes/Images/DrawLinesModel.cpp
        src/Nodes/Images/DrawLinesModel.h
        src/Nodes/Images/DrawLinesForm.ui
        src/Nodes/OpenCV/CascadeClassifier.cpp
        src/Nodes/OpenCV/CascadeClassifier.h
        src/Nodes/Variables/FileVarModel.cpp
        src/Nodes/Variables/FileVarModel.h
        src/Nodes/Variables/FileVarForm.ui
        src/Nodes/Data/FileData.h
)

target_link_libraries(OpenCVNodeEditor PRIVATE
        QtNodes::QtNodes
        opencv::opencv
        Qt6::Concurrent
)

target_include_directories(OpenCVNodeEditor PRIVATE src)
