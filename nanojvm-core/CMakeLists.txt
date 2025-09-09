
add_library(nanojvm-core STATIC
        src/nanojvm.c
        src/util/array_stack.c
        src/interpreter.c
        src/util/heap.c
        src/util/memory_region_ll.c
        src/object/allocator.c
)

target_include_directories(nanojvm-core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(nanojvm-core PROPERTIES
        POSITION_INDEPENDENT_CODE ON
)

set_target_properties(nanojvm-core PROPERTIES
        EXCLUDE_FROM_ALL TRUE
)

target_link_libraries(nanojvm-core
        PUBLIC
        nanojvm-common
        nanojvm-classfile
)

