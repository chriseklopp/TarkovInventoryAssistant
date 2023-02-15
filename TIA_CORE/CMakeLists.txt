# TIA_Core library project.
cmake_minimum_required (VERSION 3.8)

project ("TIA_CORE")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${HEADERS} ${SOURCES}
)

set(OpenCV_DIR "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build")
FIND_PACKAGE( OpenCV REQUIRED )
#find_package(wxWidgets REQUIRED COMPONENTS net core base)
#include(${wxWidgets_USE_FILE})
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(TIA_HEADERS
    "./Core/TConfig.h"
    "./Core/TCore.h"
    "./Core/TDataCatalog.h"
    "./Core/TImageReader.h"
    "./Core/TScreenCapture.h"

    "./DataSupport/TDataTypes.h"
    "./DataSupport/TEvents.h"
    "./DataSupport/TItemTypes.h"
    "./DataSupport/VPTree.h"

    "./Interfaces/TAppInterface.h"

    "./Tests/TTestLibrary.h"
)


set(TIA_SOURCES
    "./Core/TConfig.cpp"
    "./Core/TCore.cpp"
    "./Core/TDataCatalog.cpp"
    "./Core/TImageReader.cpp"
    "./Core/TScreenCapture.cpp"

    "./DataSupport/TDataTypes.cpp"
    "./DataSupport/TEvents.cpp"
    "./DataSupport/TItemTypes.cpp"
    "./DataSupport/VPTree.cpp"

    "./Interfaces/TAppInterface.cpp"

    "./Tests/TTestLibrary.cpp"
)


# Add source to this project's executable.

add_library(TIA_CORE ${TIA_HEADERS} ${TIA_SOURCES})


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TIA_CORE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )


TARGET_LINK_LIBRARIES (TIA_CORE ${OpenCV_LIBS})

set_property(TARGET TIA_CORE PROPERTY CXX_STANDARD 17)


#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
install(
    TARGETS TIA_CORE
    LIBRARY DESTINATION lib
    #ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)


