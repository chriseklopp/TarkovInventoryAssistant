# CMakeList.txt : CMake project for TarkovInventoryAssistant, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("TarkovInventoryAssistant")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${HEADERS} ${SOURCES}
)





set(OpenCV_DIR "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build")
FIND_PACKAGE( OpenCV REQUIRED )
find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(TIA_HEADERS
    "TarkovInventoryAssistant.h"
    "./DataSupport/TDataTypes.h"
    "./DataSupport/TItemTypes.h"
    "./Core/TScreenCapture.h"
    "./Core/TImageReader.h"
    "./Core/TDataCatalog.h"
    "./DataSupport/VPTree.h"
    "./UI/TUIPanels.h"
    "./Core/TCore.h"
    "./Core/TConfig.h"
    "./DataSupport/TEvents.h"
)


set(TIA_SOURCES
    "TarkovInventoryAssistant.cpp"
    "./DataSupport/TDataTypes.cpp"
    "./DataSupport/TItemTypes.cpp"
    "./Core/TScreenCapture.cpp"
    "./Core/TImageReader.cpp"
    "./Core/TDataCatalog.cpp"
    "./DataSupport/VPTree.cpp"
    "./UI/TUIPanels.cpp"
    "./Core/TCore.cpp"
    "./Core/TConfig.cpp"
    "./DataSupport/TEvents.cpp"
)


# Add source to this project's executable.
add_executable (TarkovInventoryAssistant WIN32 ${TIA_HEADERS})
target_sources (TarkovInventoryAssistant PRIVATE ${TIA_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TarkovInventoryAssistant PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

#target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Release/opencv_img_hash460.lib")
target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Debug/opencv_img_hash460d.lib")
TARGET_LINK_LIBRARIES (TarkovInventoryAssistant ${OpenCV_LIBS})
target_link_libraries(TarkovInventoryAssistant ${wxWidgets_LIBRARIES})
set_property(TARGET TarkovInventoryAssistant PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant DESTINATION "TarkovInventoryAssistant/bin")