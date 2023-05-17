# Solution Level CMake file.
cmake_minimum_required(VERSION 3.8)
project(TarkovInventoryAssistant)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
add_subdirectory(TIA_CORE_SHARED)



if(WIN32)
  add_subdirectory(TarkovInventoryAssistant_App)
  add_subdirectory(TIA_Testing)
  include_external_msproject(TarkovInventoryAssistant_Server "${TIA_SERVER_SOURCE}/TarkovInventoryAssistant_Server.csproj")
endif(WIN32)

 if(UNIX)
     add_custom_target(TarkovInventoryAssistant_Server ALL
      COMMAND dotnet build --runtime linux-arm64 ${TIA_SERVER_SOURCE}/TarkovInventoryAssistant_Server.csproj
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      COMMENT "Building TarkovInventoryAssistant_Server"
)
 endif(UNIX)

