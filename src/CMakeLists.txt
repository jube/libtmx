cmake_minimum_required(VERSION 2.8.9)
# version 2.8.9 is necessary for Qt5
# see http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html#using-qt-5-with-cmake-older-than-2-8-9

project(LIBTMX CXX)

option(TMX_RENDER "Build tmx_render" OFF)
option(TMX_DOC "Build the documentation" OFF)

include(CPackConfig.cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(TINYXML2 REQUIRED tinyxml2)

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

add_definitions(-Wall -g -O2)

if(WIN32)
  add_definitions(-std=c++0x)
else(WIN32)
  add_definitions(-std=c++11)
endif(WIN32)

include_directories(${LIBTMX_SOURCE_DIR}/include)

add_subdirectory(lib)

if(TMX_RENDER)
  find_package(Qt5Core)
  find_package(Qt5Gui)

  if(Qt5Core_FOUND)
    add_subdirectory(bin)
  endif(Qt5Core_FOUND)
endif(TMX_RENDER)

if(TMX_DOC)
  find_package(Doxygen)

  if(DOXYGEN_FOUND)
    configure_file("${LIBTMX_SOURCE_DIR}/Doxyfile.cmake" "${LIBTMX_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${LIBTMX_BINARY_DIR}/Doxyfile")
  endif(DOXYGEN_FOUND)
endif(TMX_DOC)

configure_file("${LIBTMX_SOURCE_DIR}/libtmx0.pc.in" "${LIBTMX_BINARY_DIR}/libtmx0.pc" @ONLY)
install(
  FILES "${LIBTMX_BINARY_DIR}/libtmx0.pc"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig"
)

install(
  DIRECTORY "${LIBTMX_SOURCE_DIR}/include/tmx"
  DESTINATION include
)
