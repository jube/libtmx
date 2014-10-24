include_directories(${TINYXML2_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-DZLIB_CONST)

set(LIBTMX_SRC
  Component.cc
  Layers.cc
  LayerVisitor.cc
  Map.cc
  Object.cc
  Parser.cc
  TileSet.cc
)

add_library(tmx0 SHARED
  ${LIBTMX_SRC}
)

target_link_libraries(tmx0 ${TINYXML2_LDFLAGS} ${ZLIB_LIBRARIES} ${Boost_LIBRARIES})

set_target_properties(tmx0
  PROPERTIES
  VERSION ${CPACK_PACKAGE_VERSION}
  SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
)

install(
  TARGETS tmx0
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
