add_library(libSceNgs2.native SHARED EXCLUDE_FROM_ALL
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Rack.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Render.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/System.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Unimplemented.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Voice.cpp
)
target_include_directories(libSceNgs2.native PRIVATE ${LIBS_INCLUDE_DIR})
set_target_properties(libSceNgs2.native PROPERTIES
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
)
configure_windows_unwind(libSceNgs2.native)

if(MINGW)
    foreach(runtimeLinkFlag IN ITEMS -static-libgcc -static-libstdc++ -static)
        string(REPLACE "${runtimeLinkFlag}" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    endforeach()
endif()

if(BUILD_TESTING)
    add_test_executable(ngs2_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/Ngs2Render.cpp)
    target_include_directories(ngs2_tests PRIVATE ${LIBS_INCLUDE_DIR})
    target_link_libraries(ngs2_tests PRIVATE libSceNgs2.native libkernel libc)
    configure_windows_unwind(ngs2_tests)
    add_test(NAME ngs2_render COMMAND ngs2_tests)
    if(WIN32)
        set_tests_properties(ngs2_render PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceNgs2.native>")
    endif()
endif()
