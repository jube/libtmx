include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

add_executable(tmx_render tmx_render.cc)
target_link_libraries(tmx_render tmx0 ${SFML2_LIBRARIES} ${Boost_LIBRARIES})
