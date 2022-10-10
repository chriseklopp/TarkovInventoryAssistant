# CMakeList.txt : CMake project for TarkovInventoryAssistant, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TarkovInventoryAssistant")

set(OpenCV_DIR "C:/MyWorkspace/opencv/OpenCV-4.2.0/opencv/build/x64/vc15/lib")
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

"TarkovInventoryAssistant.cpp"
"TDataTypes.cpp"
"TITemTypes.cpp"
"TScreenCapture.cpp"
"TImageReader.cpp"
"TCore.cpp"
"TDataCatalog.cpp"
)

TARGET_LINK_LIBRARIES (TarkovInventoryAssistant ${OpenCV_LIBS})

# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant DESTINATION "TarkovInventoryAssistant/bin")