# Solution Level CMake file.
cmake_minimum_required(VERSION 3.2.2)
project(TarkovInventoryAssistant)


if(NOT MSVC)
    message(FATAL_ERROR "This CMake files only works with MSVC.")
endif(NOT MSVC)


# ---------- Setup output Directories -------------------------
if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all Libraries"
    )
endif()

# --------- Setup the Executable output Directory -------------
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all Executables."
    )
endif()

# --------- Setup the Executable output Directory -------------
if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/Bin
    CACHE PATH
    "Single Directory for all static libraries."
    )
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(TIA_SERVER_SOURCE "${CMAKE_SOURCE_DIR}/TarkovInventoryAssistant_Server")
add_subdirectory(TIA_CORE)
add_subdirectory(TarkovInventoryAssistant_App)
add_subdirectory(TIA_Testing)


include_external_msproject(TarkovInventoryAssistant_Server "${TIA_SERVER_SOURCE}/TarkovInventoryAssistant_Server.csproj")
