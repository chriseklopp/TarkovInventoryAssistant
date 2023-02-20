# TIA_Core library project.
cmake_minimum_required (VERSION 3.8)

project ("TIA_CORE_SHARED")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${HEADERS} ${SOURCES}
)

set(OpenCV_DIR "C:/MyWorkspace/opencv/Opencv_4.6/opencv/build")
FIND_PACKAGE( OpenCV REQUIRED )
#find_package(wxWidgets REQUIRED COMPONENTS net core base)
#include(${wxWidgets_USE_FILE})
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(TIA_HEADERS

    "TWebInterface.h"
)


set(TIA_SOURCES
    "TWebInterface.cpp"
)


# Add source to this project's executable.

add_library(TIA_CORE_SHARED SHARED ${TIA_HEADERS} ${TIA_SOURCES})



source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TIA_CORE_SHARED PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(${CMAKE_SOURCE_DIR}/TIA_CORE)
target_link_libraries(TIA_CORE_SHARED TIA_CORE)




TARGET_LINK_LIBRARIES (TIA_CORE_SHARED ${OpenCV_LIBS})

set_property(TARGET TIA_CORE_SHARED PROPERTY CXX_STANDARD 17)

add_custom_command(TARGET TIA_CORE_SHARED POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:TIA_CORE_SHARED>"
       "${TIA_SERVER_SOURCE}/build/x64/$<CONFIG>/net6.0/"
  COMMENT "Copying to TIA_Server output directory")


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")
install(
    TARGETS TIA_CORE_SHARED
    LIBRARY DESTINATION lib
    #ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)


