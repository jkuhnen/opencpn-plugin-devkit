cmake_minimum_required(VERSION 3.15)
project({{PLUGIN_ID}} VERSION {{PLUGIN_VERSION}} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PACKAGE_NAME "{{PLUGIN_ID}}")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PLUGIN_DISPLAY_NAME "{{PLUGIN_NAME}}")
set(PLUGIN_AUTHOR "{{AUTHOR}}")
set(PLUGIN_SOURCE_URL "{{SOURCE_URL}}")
set(PACKAGE_TARBALL_URL "{{TARBALL_URL}}")
set(OCPN_API_VERSION "1.18")

find_package(wxWidgets REQUIRED COMPONENTS base core)
include(${wxWidgets_USE_FILE})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/version.h
    @ONLY
)

add_library(${PACKAGE_NAME} SHARED
    src/plugin.cpp
    src/plugin.h
    src/ui/app_style.cpp
    src/ui/app_style.h
)

target_include_directories(${PACKAGE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

add_subdirectory(opencpn-libs/api-18)
target_link_libraries(${PACKAGE_NAME} PRIVATE
    ocpn::api
    ${wxWidgets_LIBRARIES}
)

if(WIN32)
  add_subdirectory(opencpn-libs/WindowsHeaders)
  target_link_libraries(${PACKAGE_NAME} PRIVATE windows::headers)
endif()

if(MSVC)
  target_compile_definitions(${PACKAGE_NAME} PRIVATE
      MAKING_PLUGIN
      __MSVC__
      _CRT_NONSTDC_NO_DEPRECATE
      _CRT_SECURE_NO_DEPRECATE
  )
  target_compile_options(${PACKAGE_NAME} PRIVATE /utf-8)
endif()

set_target_properties(${PACKAGE_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${PACKAGE_NAME}"
)

set(OCPN_PACKAGE_TARGET "" CACHE STRING
    "OpenCPN catalog target, for example msvc-wx32")
if(NOT OCPN_PACKAGE_TARGET)
  if(MSVC)
    set(OCPN_PACKAGE_TARGET "msvc-wx32")
  else()
    set(OCPN_PACKAGE_TARGET "unknown")
  endif()
endif()

set(OCPN_TARGET_ARCH "" CACHE STRING
    "Architecture of the packaged plugin binary")
if(NOT OCPN_TARGET_ARCH)
  if(CMAKE_VS_PLATFORM_NAME)
    string(TOLOWER "${CMAKE_VS_PLATFORM_NAME}" _platform_name)
    if(_platform_name STREQUAL "win32")
      set(OCPN_TARGET_ARCH "x86")
    elseif(_platform_name STREQUAL "x64")
      set(OCPN_TARGET_ARCH "x86_64")
    elseif(_platform_name STREQUAL "arm64")
      set(OCPN_TARGET_ARCH "arm64")
    else()
      set(OCPN_TARGET_ARCH "${_platform_name}")
    endif()
  else()
    set(OCPN_TARGET_ARCH "unknown")
  endif()
endif()

set(OCPN_TARGET_VERSION "" CACHE STRING
    "Target Windows/OS version recorded in plugin metadata")
if(NOT OCPN_TARGET_VERSION)
  if(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION)
    set(OCPN_TARGET_VERSION "${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")
  elseif(CMAKE_SYSTEM_VERSION)
    set(OCPN_TARGET_VERSION "${CMAKE_SYSTEM_VERSION}")
  else()
    set(OCPN_TARGET_VERSION "unknown")
  endif()
endif()

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")
set(CPACK_PACKAGE_FILE_NAME
    "${PACKAGE_NAME}-${PACKAGE_VERSION}-${OCPN_PACKAGE_TARGET}-${OCPN_TARGET_ARCH}-${OCPN_TARGET_VERSION}"
)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/metadata.xml.in
    ${CMAKE_CURRENT_BINARY_DIR}/metadata.xml
    @ONLY
)

install(TARGETS ${PACKAGE_NAME}
    RUNTIME DESTINATION plugins
    LIBRARY DESTINATION plugins
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/metadata.xml DESTINATION .)

message(STATUS "OpenCPN API: ${OCPN_API_VERSION}")
message(STATUS "Package target: ${OCPN_PACKAGE_TARGET}")
message(STATUS "Package architecture: ${OCPN_TARGET_ARCH}")
message(STATUS "Package target version: ${OCPN_TARGET_VERSION}")

include(CPack)
