include_directories ("${PROJECT_SOURCE_DIR}/src")

add_executable(test_render test_render.c)
add_executable(test_widget test_widget.c)

add_test(test_render test_render)
add_test(test_widget test_widget)

target_link_libraries(test_render video cmocka SDL2main)
target_link_libraries(test_widget widget cmocka SDL2main)

IF (WIN32)
    set(MY_CMOCKA_LIB_NAME "cmocka.dll")
ENDIF()

if (MY_CMOCKA_LIB_NAME)
    message("copy from ${CMAKE_CURRENT_BINARY_DIR}/../_deps/cmocka-build/src/${MY_CMOCKA_LIB_NAME} to ${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_command(TARGET test_render POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_BINARY_DIR}/../_deps/cmocka-build/src/${MY_CMOCKA_LIB_NAME}"
            ${CMAKE_CURRENT_BINARY_DIR})
ENDIF()