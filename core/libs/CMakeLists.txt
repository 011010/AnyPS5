if(NOT WIN32)
    string(REPLACE "-static " "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")
    string(STRIP "${CMAKE_EXE_LINKER_FLAGS}" CMAKE_EXE_LINKER_FLAGS)
endif()

add_executable(nid_patcher
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/Sha1.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/NidCompute.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/NidResolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/ExportExclusions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/ElfNidPatcher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/PeNidPatcher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/BinaryPatcherFactory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/nid/src/main.cpp
)

target_include_directories(nid_patcher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/nid/include)
target_compile_features(nid_patcher PRIVATE cxx_std_20)

if(MINGW)
    target_link_options(nid_patcher PRIVATE -static -static-libgcc -static-libstdc++)
    add_compile_options(-fno-asynchronous-unwind-tables)
endif()

function(configure_windows_unwind target)
    if(MINGW)
        if(NOT EXISTS "${CMAKE_OBJCOPY}")
            message(FATAL_ERROR "Windows DWARF unwinding requires objcopy")
        endif()
        add_custom_command(TARGET ${target} POST_BUILD
                COMMAND "${CMAKE_OBJCOPY}" --rename-section .eh_frame=.ehfram "$<TARGET_FILE:${target}>"
                VERBATIM
        )
    endif()
endfunction()

set(LIBC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prx/libc)
add_library(libc SHARED EXCLUDE_FROM_ALL)
set(LIBS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(${LIBC_DIR} ${CMAKE_CURRENT_BINARY_DIR}/prx/libc)
target_include_directories(libc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
configure_windows_unwind(libc)

set(UNPATCHED_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs/unpatched)
set(PATCHED_DIR ${CMAKE_CURRENT_BINARY_DIR}/libs)
set(IMPLIB_DIR ${PATCHED_DIR}/implib)

set_target_properties(libc PROPERTIES
        PREFIX ""
        SUFFIX .prx
        RUNTIME_OUTPUT_DIRECTORY ${UNPATCHED_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${UNPATCHED_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${IMPLIB_DIR}
)

set(LIBC_UNPATCHED ${UNPATCHED_DIR}/libc.prx)
set(LIBC_PATCHED ${PATCHED_DIR}/libc.prx)

add_custom_command(
        OUTPUT ${LIBC_PATCHED}
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBC_UNPATCHED} ${LIBC_PATCHED}
        COMMAND $<TARGET_FILE:nid_patcher> libc ${LIBC_PATCHED}
        DEPENDS nid_patcher libc ${LIBC_UNPATCHED}
        COMMENT "Patching NIDs for libc"
)

file(GLOB LIBS_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/prx/*)
list(REMOVE_ITEM LIBS_DIRS ${LIBC_DIR})

set(PATCHED_OUTPUTS ${LIBC_PATCHED})

foreach(LIB_DIR ${LIBS_DIRS})
    if(IS_DIRECTORY ${LIB_DIR})
        get_filename_component(LIB_NAME ${LIB_DIR} NAME)

        if(EXISTS ${LIB_DIR}/CMakeLists.txt)
            set(LIBS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
            add_subdirectory(${LIB_DIR} ${CMAKE_CURRENT_BINARY_DIR}/prx/${LIB_NAME})
        else()
            add_library(${LIB_NAME} SHARED EXCLUDE_FROM_ALL ${LIB_DIR}/Export.cpp)
            target_include_directories(${LIB_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
            configure_windows_unwind(${LIB_NAME})
        endif()

        if(LIB_NAME STREQUAL "libkernel")
            target_link_libraries(${LIB_NAME} PRIVATE libc)
        else()
            target_link_libraries(${LIB_NAME} PRIVATE libkernel libc)
        endif()
        if(WIN32 AND LIB_NAME STREQUAL "libSceNet")
            target_link_libraries(${LIB_NAME} PRIVATE ws2_32)
            target_compile_definitions(${LIB_NAME} PRIVATE _WIN32_WINNT=0x0602)
        elseif(WIN32 AND LIB_NAME STREQUAL "libSceNetCtl")
            target_link_libraries(${LIB_NAME} PRIVATE ws2_32 iphlpapi)
            target_compile_definitions(${LIB_NAME} PRIVATE _WIN32_WINNT=0x0602)
        endif()

        set_target_properties(${LIB_NAME} PROPERTIES
                PREFIX ""
                SUFFIX .prx
                RUNTIME_OUTPUT_DIRECTORY ${UNPATCHED_DIR}
                LIBRARY_OUTPUT_DIRECTORY ${UNPATCHED_DIR}
                ARCHIVE_OUTPUT_DIRECTORY ${IMPLIB_DIR}
        )

        set(UNPATCHED_FILE ${UNPATCHED_DIR}/${LIB_NAME}.prx)
        set(PATCHED_FILE ${PATCHED_DIR}/${LIB_NAME}.prx)

        add_custom_command(
                OUTPUT ${PATCHED_FILE}
                COMMAND ${CMAKE_COMMAND} -E copy ${UNPATCHED_FILE} ${PATCHED_FILE}
                COMMAND $<TARGET_FILE:nid_patcher> ${LIB_NAME} --preserve-exports "${LIBC_UNPATCHED}" "${PATCHED_FILE}"
                DEPENDS nid_patcher ${LIB_NAME} ${UNPATCHED_FILE} ${LIBC_UNPATCHED}
                COMMENT "Patching NIDs for ${LIB_NAME}"
        )

        list(APPEND PATCHED_OUTPUTS ${PATCHED_FILE})
    endif()
endforeach()

set(LIBCOHTML_PATCHED ${PATCHED_DIR}/libcohtml.prx)
set(LIBCOHTML_PROSPERO_PATCHED ${PATCHED_DIR}/libcohtml.Prospero.prx)
add_custom_command(
        OUTPUT ${LIBCOHTML_PROSPERO_PATCHED}
        COMMAND ${CMAKE_COMMAND} -E copy "${LIBCOHTML_PATCHED}" "${LIBCOHTML_PROSPERO_PATCHED}"
        DEPENDS ${LIBCOHTML_PATCHED}
        VERBATIM
)
add_custom_target(libcohtml_prospero DEPENDS ${LIBCOHTML_PROSPERO_PATCHED})
list(APPEND PATCHED_OUTPUTS ${LIBCOHTML_PROSPERO_PATCHED})

add_custom_target(libs DEPENDS ${PATCHED_OUTPUTS})

if(BUILD_TESTING)
    add_library(host_thread_local_test_library SHARED tests/HostThreadLocalLibrary.cpp)
    set_target_properties(host_thread_local_test_library PROPERTIES RUNTIME_OUTPUT_DIRECTORY "$<1:${ANYPS5_TEST_OUTPUT_DIRECTORY}>")
    target_include_directories(host_thread_local_test_library PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test_executable(host_thread_local_tests tests/HostThreadLocal.cpp)
    target_link_libraries(host_thread_local_tests PRIVATE host_thread_local_test_library)
    if(MINGW)
        target_compile_options(host_thread_local_test_library PRIVATE -fasynchronous-unwind-tables)
        target_compile_options(host_thread_local_tests PRIVATE -fasynchronous-unwind-tables)
    endif()
    add_test(NAME host_thread_local COMMAND host_thread_local_tests)
    set_tests_properties(host_thread_local PROPERTIES TIMEOUT 15)
    add_test_executable(guest_system_exit_tests tests/GuestSystemExit.cpp)
    target_include_directories(guest_system_exit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_system_exit_tests PRIVATE libSceSystemService libc)
    configure_windows_unwind(guest_system_exit_tests)
    add_test(NAME guest_system_exit_parameters COMMAND guest_system_exit_tests)
    add_test(NAME guest_system_exit COMMAND guest_system_exit_tests --exit)
    add_test(NAME guest_immediate_exit COMMAND guest_system_exit_tests --immediate)
    add_test(NAME guest_return_from_main COMMAND guest_system_exit_tests --return-from-main)
    set_tests_properties(guest_system_exit PROPERTIES
        PASS_REGULAR_EXPRESSION "Guest shutdown and atexit completed")
    if(WIN32)
        set_tests_properties(guest_system_exit guest_system_exit_parameters guest_immediate_exit guest_return_from_main PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libc>")
    endif()
    add_test_executable(guest_system_service_tests tests/GuestSystemService.cpp)
    target_include_directories(guest_system_service_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_system_service_tests PRIVATE libSceSystemService libc)
    configure_windows_unwind(guest_system_service_tests)
    add_test(NAME guest_system_service COMMAND guest_system_service_tests)
    if(WIN32)
        set_tests_properties(guest_system_service PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libc>")
    endif()
    add_test_executable(guest_content_export_tests tests/GuestContentExport.cpp)
    target_include_directories(guest_content_export_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_content_export_tests PRIVATE libSceContentExport libc)
    configure_windows_unwind(guest_content_export_tests)
    add_test(NAME guest_content_export COMMAND guest_content_export_tests)
    if(WIN32)
        set_tests_properties(guest_content_export PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libc>")
    endif()
    add_test_executable(guest_sce_net_tests tests/GuestSceNet.cpp)
    target_include_directories(guest_sce_net_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_sce_net_tests PRIVATE libSceNet libSceNetCtl)
    configure_windows_unwind(guest_sce_net_tests)
    add_test(NAME guest_sce_net COMMAND guest_sce_net_tests)
    set_tests_properties(guest_sce_net PROPERTIES TIMEOUT 20)
    if(WIN32)
        set_tests_properties(guest_sce_net PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceNet>")
    endif()
    add_test_executable(guest_random_tests tests/GuestRandom.cpp)
    target_include_directories(guest_random_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_random_tests PRIVATE libSceRandom)
    add_test(NAME guest_random COMMAND guest_random_tests)
    if(WIN32)
        set_tests_properties(guest_random PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceRandom>")
    endif()
    add_test_executable(guest_rtc_tests tests/GuestRtc.cpp)
    target_include_directories(guest_rtc_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_rtc_tests PRIVATE libSceRtc)
    add_test(NAME guest_rtc COMMAND guest_rtc_tests)
    if(WIN32)
        set_tests_properties(guest_rtc PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceRtc>")
    endif()
    add_test_executable(guest_json_tests tests/GuestJson.cpp)
    target_include_directories(guest_json_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(guest_json_tests PRIVATE libSceJson2 libc)
    add_test(NAME guest_json COMMAND guest_json_tests)
    if(WIN32)
        set_tests_properties(guest_json PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceJson2>")
    endif()
    add_test_executable(fiber_tests tests/Fiber.cpp)
    target_include_directories(fiber_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(fiber_tests PRIVATE libSceFiber libc)
    add_test(NAME fiber COMMAND fiber_tests)
    if(WIN32)
        set_tests_properties(fiber PROPERTIES ENVIRONMENT_MODIFICATION
            "PATH=path_list_prepend:$<TARGET_FILE_DIR:libSceFiber>")
    endif()
endif()

if(MINGW)
    foreach(runtimeLinkFlag IN ITEMS -static-libgcc -static-libstdc++ -static)
        string(REPLACE "${runtimeLinkFlag}" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    endforeach()
endif()
