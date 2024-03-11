//
// Created by pablo on 2/24/24.
//

#ifndef NODESINCLUDE_H
#define NODESINCLUDE_H

#include <opencv2/opencv.hpp>

#include "Nodes/Constants/PiModel.h"

#include "Nodes/Basic/AdditionModel.h"
#include "Nodes/Basic/DivisionModel.h"
#include "Nodes/Basic/MathOperationDataModel.hpp"
#include "Nodes/Basic/MultiplicationModel.hpp"
#include "Nodes/Basic/NumberDisplayDataModel.hpp"
#include "Nodes/Basic/NumberSourceDataModel.hpp"
#include "Nodes/Basic/SubtractionModel.h"

#include "Nodes/Images/ImageLoaderModel.hpp"
#include "Nodes/Images/ImageShowModel.hpp"
#include "Nodes/Images/ImageInfoModel.h"
#include "Nodes/Images/DrawLinesModel.h"
#include "Nodes/Images/DrawRectsModel.h"
#include "Nodes/Images/ConvertImageToModel.h"
#include "Nodes/Images/ScaleImageModel.h"
#include "Nodes/Images/CutImageModel.h"
// Variables
#include "Nodes/Variables/SizeVarModel.h"
#include "Nodes/Variables/FileVarModel.h"
#include "Nodes/Variables/RectVarModel.h"
#include "Nodes/Variables/RectModel.h"

// DataOperations
#include "Nodes/DataOperations/ScaleRects.h"

// opencv
#include "Nodes/OpenCV/ColorCVModel.h"
#include "Nodes/OpenCV/BlurModel.h"
#include "Nodes/OpenCV/CannyModel.h"
#include "Nodes/OpenCV/GaussianBlurModel.h"
#include "Nodes/OpenCV/HoughLinesPModel.h"
#include "Nodes/OpenCV/CascadeClassifierModel.h"
#include "Nodes/OpenCV/EqualizeHistModel.h"
#include "Nodes/OpenCV/DetectMultiScaleModel.h"
#include "Nodes/OpenCV/PyrDown.h"

// video
#include "Nodes/Video/CameraModel.h"
#include "Nodes/Video/CaptureModel.h"

// data
#include "Nodes/Data/DataInclude.h"

// Conversor
#include "Nodes/Conversor/MatQt.h"


#endif //NODESINCLUDE_H
