# CMakeList.txt : CMake project for TarkovInventoryAssistant, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TarkovInventoryAssistant")

set(OpenCV_DIR "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build")
FIND_PACKAGE( OpenCV REQUIRED )
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Add source to this project's executable.
add_executable (TarkovInventoryAssistant WIN32

"TarkovInventoryAssistant.h"
"TDataTypes.h"
"TItemTypes.h"
"TScreenCapture.h"
"TImageReader.h"
"TDataCatalog.h"
"VPTree.h"
"TUIPanels.h"
"TCore.h"
"TConfig.h"
"TEvents.h"

"TarkovInventoryAssistant.cpp"
"TDataTypes.cpp"
"TITemTypes.cpp"
"TScreenCapture.cpp"
"TImageReader.cpp"
"TDataCatalog.cpp"
"VPTree.cpp"
"TUIPanels.cpp"
"TCore.cpp"
"TConfig.cpp"
"TEvents.cpp"
)


target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Debug/opencv_img_hash460d.lib")
TARGET_LINK_LIBRARIES (TarkovInventoryAssistant ${OpenCV_LIBS})
target_link_libraries(TarkovInventoryAssistant ${wxWidgets_LIBRARIES})
set_property(TARGET TarkovInventoryAssistant PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant DESTINATION "TarkovInventoryAssistant/bin")