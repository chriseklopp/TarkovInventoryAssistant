# TarkovInventoryAssistant_App project. 
# Desktop app project.
cmake_minimum_required (VERSION 3.8)

project ("TarkovInventoryAssistant_App")

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

    "./UI/CatalogPanel.h"
    "./UI/ConsolePanel.h"
    "./UI/DetectionValidatorWidget.h"
    "./UI/DisplayPanel.h"
    "./UI/OutputPanel.h"
    "./UI/UISupport.h"
    "./UI/TUIPanels.h"

)


set(TIA_SOURCES
    "TarkovInventoryAssistant.cpp"

    "./UI/CatalogPanel.cpp"
    "./UI/ConsolePanel.cpp"
    "./UI/DetectionValidatorWidget.cpp"
    "./UI/DisplayPanel.cpp"
    "./UI/OutputPanel.cpp"
    "./UI/UISupport.cpp"
    "./UI/TUIPanels.cpp"

)


# Add source to this project's executable.
add_executable (TarkovInventoryAssistant_App WIN32 ${TIA_HEADERS})
target_sources (TarkovInventoryAssistant_App PRIVATE ${TIA_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TarkovInventoryAssistant_App PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )


include_directories(${CMAKE_SOURCE_DIR}/TIA_CORE)
target_link_libraries(TarkovInventoryAssistant_App TIA_CORE)

TARGET_LINK_LIBRARIES (TarkovInventoryAssistant_App ${OpenCV_LIBS})
target_link_libraries(TarkovInventoryAssistant_App ${wxWidgets_LIBRARIES})
set_property(TARGET TarkovInventoryAssistant_App PROPERTY CXX_STANDARD 17)

# TODO: Add tests and install targets if needed.
install (TARGETS TarkovInventoryAssistant_App DESTINATION "TarkovInventoryAssistant/bin")