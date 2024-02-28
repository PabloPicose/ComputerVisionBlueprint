//
// Created by pablo on 2/24/24.
//

#ifndef NODESINCLUDE_H
#define NODESINCLUDE_H

#include <opencv2/opencv.hpp>

#include "Nodes/Constants/PiModel.h"

#include "Nodes/Basic/AdditionModel.hpp"
#include "Nodes/Data/DecimalData.hpp"
#include "Nodes/Basic/DivisionModel.hpp"
#include "Nodes/Basic/MathOperationDataModel.hpp"
#include "Nodes/Basic/MultiplicationModel.hpp"
#include "Nodes/Basic/NumberDisplayDataModel.hpp"
#include "Nodes/Basic/NumberSourceDataModel.hpp"
#include "Nodes/Basic/SubtractionModel.hpp"

#include "Nodes/Images/ImageLoaderModel.hpp"
#include "Nodes/Images/ImageShowModel.hpp"

// opencv
#include "Nodes/OpenCV/ColorCV.h"
#include "Nodes/OpenCV/BlurModel.h"
#include "Nodes/OpenCV/CannyModel.h"
#include "Nodes/OpenCV/GaussianBlurModel.h"
#include "Nodes/OpenCV/HoughLinesP.h"

// data
#include "Nodes/Data/DataInclude.h"

// Conversor
#include "Nodes/Conversor/MatQt.h"


#endif //NODESINCLUDE_H
