
add_library(nanojvm-classfile STATIC
    src/reader.c
    src/utils.c
    src/attribute.c
    src/constant_pool.c
    src/field.c
    src/method.c
)

target_include_directories(nanojvm-classfile
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(nanojvm-classfile PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

set_target_properties(nanojvm-classfile PROPERTIES
    EXCLUDE_FROM_ALL TRUE
)

target_link_libraries(nanojvm-classfile
    PUBLIC
        nanojvm-common
)

