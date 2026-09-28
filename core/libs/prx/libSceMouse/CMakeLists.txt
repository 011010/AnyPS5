add_library(libSceMouse SHARED EXCLUDE_FROM_ALL
    ${CMAKE_CURRENT_SOURCE_DIR}/Export.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse_impl.cpp
)
target_include_directories(libSceMouse PRIVATE ${LIBS_INCLUDE_DIR})
set_target_properties(libSceMouse PROPERTIES CXX_EXTENSIONS OFF)
configure_windows_unwind(libSceMouse)
