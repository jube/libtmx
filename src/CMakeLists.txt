cmake_minimum_required(VERSION 2.6)

project(LIBTMX CXX)

include(CPackConfig.cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(TINYXML2 REQUIRED tinyxml2)

find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)

add_definitions(-Wall -g -O2 -std=c++11)

add_subdirectory(lib)

configure_file("${LIBTMX_SOURCE_DIR}/libtmx0.pc.in" "${LIBTMX_BINARY_DIR}/libtmx0.pc" @ONLY)
install(
  FILES "${LIBTMX_BINARY_DIR}/libtmx0.pc"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/pkgconfig"
)

install(
  DIRECTORY "${LIBTMX_SOURCE_DIR}/include/tmx"
  DESTINATION include
)
