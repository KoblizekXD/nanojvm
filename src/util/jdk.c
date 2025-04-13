#include <nanojvm.h>
#include <stdlib.h>
#include <util/logging.h>
#include <miniz.h>

mz_zip_archive *attempt_java11(const char *home)
{
    char *mod_path = malloc(strlen(home) + 22);
    strcpy(mod_path, home);
    strcat(mod_path, "/jmods/java.base.jmod");
    mz_zip_archive *zip = calloc(1, sizeof(mz_zip_archive));
    if (!mz_zip_reader_init_file(zip, mod_path, 0)) {
        debug("Assuming < Java 11");
        free(mod_path);
        free(zip);
        return NULL;
    } else {
        free(mod_path);
        return zip;
    }
}

mz_zip_archive *attempt_java_leg(const char *home)
{
    char *mod_path = malloc(strlen(home) + 16);
    strcpy(mod_path, home);
    strcat(mod_path, "/jre/lib/rt.jar");
    mz_zip_archive *zip = calloc(1, sizeof(mz_zip_archive));
    if (!mz_zip_reader_init_file(zip, mod_path, 0)) {
        free(mod_path);
        free(zip);
        return NULL;
    }
    free(mod_path);
    return zip;
}

JDK *SetupJDK(void)
{
    char *home = getenv("JAVA_HOME");

    if (!home) {
        error("Cannot load JDK information, please set JAVA_HOME environment variable");
        return NULL;
    }

    JDK *jdk = malloc(sizeof(JDK));
    jdk->path = home;

    jdk->handle = attempt_java11(home);

    if (!jdk->handle) {
        jdk->handle = attempt_java_leg(home);
        if (!jdk->handle)
            jdk->mode = 0;
        else jdk->mode = 8;
    } else jdk->mode = 11;
    if (jdk->mode == 0) warn("Failed to correctly determine JDK information. Please report this as bug with your JDK installation information");
    debug("Using %s", jdk->path);
    return jdk;
}
