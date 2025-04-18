#include "miniz.h"
#include <alloca.h>
#include <ctype.h>
#include <nanojvm.h>
#include <stddef.h>
#include <util/logging.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vmopts.h>
#include <util/strings.h>

void PrintHelp(void)
{
    printf(
        "Usage: nanojvm [options] <main> [args...]\n"
        "Arguments passed after main class are passed to the application.\n"
        "Options:\n"
        "\t--classpath <directory/file>\n"
        "\t\tAppends a file or a directory to the runtime classpath. If file is passed, it needs to be one of: .class, .jar, .jmod. This option can be repeated.\n"
        "\t--no-jdk\n"
        "\t\tDisables default lookup JVM installation lookup.\n"
        "\t--jdk <rt.jar>\n"
        "\t\tSpecifies path to custom JDK runtime library.\n"
        "\t--no-clinit\n"
        "\t\tDon't run static initializers\n"
        "\t--no-init\n"
        "\t\tDon't run object initializers(contructors)\n"
        "\t--heap-init <size>\n"
        "\t\tInitial heap pre-allocation size. Defaults to 4KiB.\n"
        "\t--heap-max <size>\n"
        "\t\tThe maximum size heap can expand to. Defaults to 20KiB.\n"
        "\t--help\n"
        "\t\tPrints this help message to stdout.\n"
        "\t--version\n"
        "\t\tPrints version information about current version of NanoJVM.\n"
    );
}

void PrintVersionInformation(void)
{
#ifdef __linux__
    const char *platform = "linux";
#elif defined(_WIN32)
    const char *platform = "win32";
#elif defined(__APPLE__)
    const char *platform = "macos";
#elif defined(__EMSCRIPTEN__)
    const char *platform = "wasm";
#else
    const char *platform = "undefined";
#endif
    printf("NanoJVM version %s\nBuilt at: %s %s for %s-%d-bit\nLicensed under MIT license.\n", NANOJVM_REVISION, __DATE__, __TIME__, platform, (int)(8 * sizeof(void*)));
}

const VmOptions DEFAULT_OPTIONS = (VmOptions) {
    .argc = 0,
    .argv = NULL,
    .classpath = NULL,
    .classpath_len = 0,
    .flags = 0,
    .heap_init = DEFAULT_HEAP_INITIAL,
    .heap_max = DEFAULT_HEAP_MAX,
    .jdk = NULL
};

size_t unit_parser(const char *str)
{
    char *endptr;
    double value = strtod(str, &endptr);
    
    if (endptr == str) {
        error("Invalid size: '%s'", str);
        exit(EXIT_FAILURE);
    }

    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    if (*endptr != '\0') {
        switch (tolower((unsigned char)*endptr)) {
            case 'k': value *= 1024; break;
            case 'm': value *= 1024 * 1024; break;
            case 'g': value *= 1024 * 1024 * 1024; break;
            case 't': value *= 1024ULL * 1024 * 1024 * 1024; break;
            default:
                error("Unknown size unit: '%c'", *endptr);
                exit(EXIT_FAILURE);
        }

        if (tolower((unsigned char)endptr[1]) == 'i' && 
            tolower((unsigned char)endptr[2]) == 'b') {
            endptr += 3;
        }
        else if (tolower((unsigned char)endptr[1]) == 'b') {
            endptr += 2;
        }
        else {
            endptr += 1;
        }
    }

    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    if (*endptr != '\0') {
        error("Trailing characters after size: '%s'\n", endptr);
        exit(EXIT_FAILURE);
    }

    return (size_t)value;
}

#define CheckNext if (i >= argc - 1) { error("Missing value after %s", argv[i]); exit(1); }

VmOptions *Parse(int argc, char **argv)
{
    VmOptions opts = DEFAULT_OPTIONS;

    if (argc == 1) {
        error("No input files. See --help for more information.");
        exit(1);
    }

    if (StringEquals(argv[1], "--version") || StringEquals(argv[1], "-v")) {
        PrintVersionInformation();
        exit(0);
    } else if (StringEquals(argv[1], "--help") || StringEquals(argv[1], "-h")) {
        PrintHelp();
        exit(0);
    }

    int ended = 1;
    for (int i = 1; i <= argc; i++) {
        if (i == argc || argv[i][0] != '-') {
            ended = i;
            break;
        }

        const char *str = argv[i];
        if (StringEquals(str, "--classpath") || StringEquals(str, "-c")) {
            CheckNext;
            i++;
            opts.classpath_len++;
            opts.classpath = realloc(opts.classpath, sizeof(char*) * opts.classpath_len);
            opts.classpath[opts.classpath_len - 1] = argv[i];
        } else if (StringEquals(str, "--heap-init")) {
            CheckNext;
            i++;
            opts.heap_init = unit_parser(argv[i]);
        } else if (StringEquals(str, "--heap-max")) {
            CheckNext;
            i++;
            opts.heap_max = unit_parser(argv[i]);
        } else if (StringEquals(str, "--no-jdk")) {
            opts.flags |= OPTION_DISABLE_JVM_LOOKUP;
        } else if (StringEquals(str, "--no-clinit")) {
            opts.flags |= OPTION_DISABLE_CLINIT;
        } else if (StringEquals(str, "--no-init")) {
            opts.flags |= OPTION_DISABLE_INIT;
        } else if (StringEquals(str, "--jdk")) {
            CheckNext;
            i++;
            opts.jdk = malloc(sizeof(JDK));
            opts.jdk->mode = 8;
            opts.jdk->path = argv[i];
            mz_zip_archive *zip = calloc(1, sizeof(mz_zip_archive));
            if (!mz_zip_reader_init_file(zip, argv[i], 0)) {
                free(zip);
                error("Target JDK runtime is not a valid zip format");
                exit(1);
            }
            opts.jdk->handle = zip;
        } else {
            error("Unrecognized option: %s", str);
            FreeOptionsIfPossible(&opts);
            exit(1);
        }
    }

    if (ended >= argc) {
        error("No input file to run.");
        FreeOptionsIfPossible(&opts);
        exit(1);
    }

    opts.classpath_len++;
    opts.classpath = realloc(opts.classpath, opts.classpath_len * sizeof(char*));
    opts.classpath[opts.classpath_len - 1] = argv[ended];
    ended++;
    opts.argv = argv + ended;
    opts.argc = argc - ended;

    VmOptions *allocated = malloc(sizeof(VmOptions));
    *allocated = opts;
    return allocated;
}

void FreeOptionsIfPossible(VmOptions *opts)
{
    free(opts->classpath);
}

void PrintSummary(VmOptions *options)
{
    debug("VM Options Summary:");
    debug("Heap: initial size: %llu bytes, can expand up to: %llu bytes", options->heap_init, options->heap_max);
    debug("Disable JDK loading: %s", (options->flags & OPTION_DISABLE_JVM_LOOKUP) != 0 ? "yes" : "no");
    debug("%llu classpath entries:", options->classpath_len);
    for (size_t i = 0; i < options->classpath_len; i++) {
        debug("\t%s", options->classpath[i]);
    }
    debug("%llu args:", options->argc);
    for (size_t i = 0; i < options->argc; i++) {
        debug("\t%s", options->argv[i]);
    }
}

VmOptions *GetDefaultOptions(void)
{
    VmOptions *opts = malloc(sizeof(VmOptions));
    *opts = DEFAULT_OPTIONS;
    return opts;
}
