# TIA_Testing  project.
cmake_minimum_required (VERSION 3.8)

project ("TIA_Testing")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${HEADERS} ${SOURCES}
)


set(TIA_HEADERS
	"Testing.h"
)


set(TIA_SOURCES
	"Testing.cpp"
)


# Add source to this project's executable.
add_executable (TIA_Testing  ${TIA_HEADERS})

target_sources (TIA_Testing PRIVATE ${TIA_SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TIA_HEADERS})

target_include_directories(TIA_Testing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(${CMAKE_SOURCE_DIR}/TIA_CORE)
target_link_libraries(TIA_Testing TIA_CORE)


set_property(TARGET TIA_Testing PROPERTY CXX_STANDARD 17)
# TODO: Add tests and install targets if needed.
install (TARGETS TIA_Testing DESTINATION "TarkovInventoryAssistant/bin")
