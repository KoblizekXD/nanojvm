include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

add_executable(nanojvm-tests
        src/test_main.c
        src/test_classfile.c
        src/test_array_stack.c
)

target_link_libraries(nanojvm-tests
        PUBLIC
        nanojvm-common
        nanojvm-classfile
        nanojvm-core
)

enable_testing()
add_test(NAME nanojvm-tests
        COMMAND nanojvm-tests
)

file(COPY java DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
