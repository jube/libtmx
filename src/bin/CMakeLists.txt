include_directories(${Boost_INCLUDE_DIRS})

add_executable(tmx_render tmx_render.cc)
qt5_use_modules(tmx_render Core Gui)
target_link_libraries(tmx_render tmx0 ${Boost_LIBRARIES})
