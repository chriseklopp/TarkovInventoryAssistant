# Solution Level CMake file.
cmake_minimum_required(VERSION 3.2.2)
project(TarkovInventoryAssistant)


if(NOT MSVC)
    message(FATAL_ERROR "This CMake files only works with MSVC.")
endif(NOT MSVC)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
add_subdirectory(TIA_CORE)
add_subdirectory(TarkovInventoryAssistant_App)
#add_subdirectory(TarkovInventoryAssistant_Server)
include_external_msproject(TarkovInventoryAssistant_Server "C:/MyWorkspace/TarkovInventoryAssistant/TarkovInventoryAssistant_Server/TarkovInventoryAssistant_Server.csproj")