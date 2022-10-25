# CMakeList.txt : CMake project for TarkovInventoryAssistant, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TarkovInventoryAssistant")

set(OpenCV_DIR "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build")
FIND_PACKAGE( OpenCV REQUIRED )   
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Add source to this project's executable.
add_executable (TarkovInventoryAssistant 

"TarkovInventoryAssistant.h"
"TDataTypes.h"
"TItemTypes.h"
"TScreenCapture.h"
"TImageReader.h"
"TCore.h"
"TDataCatalog.h"
"VPTree.h"
"TGlobal.h"

"TarkovInventoryAssistant.cpp"
"TDataTypes.cpp"
"TITemTypes.cpp"
"TScreenCapture.cpp"
"TImageReader.cpp"
"TCore.cpp"
"TDataCatalog.cpp"
"VPTree.cpp"
"TGlobal.cpp"
)
target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Debug/opencv_img_hash460d.lib")
TARGET_LINK_LIBRARIES (TarkovInventoryAssistant ${OpenCV_LIBS})
set_property(TARGET TarkovInventoryAssistant PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant DESTINATION "TarkovInventoryAssistant/bin")