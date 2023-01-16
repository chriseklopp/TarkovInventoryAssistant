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
    "./Core/TConfig.h"
    "./Core/TCore.h"
    "./Core/TDataCatalog.h"
    "./Core/TImageReader.h"
    "./Core/TScreenCapture.h"

    "./DataSupport/TDataTypes.h"
    "./DataSupport/TEvents.h"
    "./DataSupport/TItemTypes.h"
    "./DataSupport/VPTree.h"

    "./UI/CatalogPanel.h"
    "./UI/ConsolePanel.h"
    "./UI/DetectionValidatorWidget.h"
    "./UI/DisplayPanel.h"
    "./UI/OutputPanel.h"
    "./UI/UISupport.h"
    "./UI/TUIPanels.h"

    "./Tests/TTestLibrary.h"
)


set(TIA_SOURCES
    "TarkovInventoryAssistant.cpp"
    "./Core/TConfig.cpp"
    "./Core/TCore.cpp"
    "./Core/TDataCatalog.cpp"
    "./Core/TImageReader.cpp"
    "./Core/TScreenCapture.cpp"

    "./DataSupport/TDataTypes.cpp"
    "./DataSupport/TEvents.cpp"
    "./DataSupport/TItemTypes.cpp"
    "./DataSupport/VPTree.cpp"

    "./UI/CatalogPanel.cpp"
    "./UI/ConsolePanel.cpp"
    "./UI/DetectionValidatorWidget.cpp"
    "./UI/DisplayPanel.cpp"
    "./UI/OutputPanel.cpp"
    "./UI/UISupport.cpp"
    "./UI/TUIPanels.cpp"

    "./Tests/TTestLibrary.cpp"
)


# Add source to this project's executable.
add_executable (TarkovInventoryAssistant WIN32 ${TIA_HEADERS})
target_sources (TarkovInventoryAssistant PRIVATE ${TIA_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TarkovInventoryAssistant PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

#target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Release/opencv_img_hash460.lib")
#target_link_libraries(TarkovInventoryAssistant "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build/lib/Debug/opencv_img_hash460d.lib")
TARGET_LINK_LIBRARIES (TarkovInventoryAssistant ${OpenCV_LIBS})
target_link_libraries(TarkovInventoryAssistant ${wxWidgets_LIBRARIES})
set_property(TARGET TarkovInventoryAssistant PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant DESTINATION "TarkovInventoryAssistant/bin")