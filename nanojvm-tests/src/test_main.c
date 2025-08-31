#include <greatest.h>

SUITE_EXTERN(classfile_suite);

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(classfile_suite);

    GREATEST_MAIN_END();
}