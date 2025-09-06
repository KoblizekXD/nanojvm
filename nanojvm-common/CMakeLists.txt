
add_library(nanojvm-common STATIC
        src/strrchr.c
        src/logging.c
        src/reader.c
        src/strlen.c
        src/djb2.c
        src/memory.c
        src/memcmp.c
)

target_include_directories(nanojvm-common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(nanojvm-common PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

set_target_properties(nanojvm-common PROPERTIES
    EXCLUDE_FROM_ALL TRUE
)

