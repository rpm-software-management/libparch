#include <stdlib.h>
#include <string.h>

#include "libarch/arch.h"

#include "list.h"


// -pedantic-errors option checks if the numbers aren't too small
#define MAX_ARCHES 10


typedef struct _ArchInfo {
    struct _Arch * arch_list;
    struct _Arch * this_arch;
    char * arch_name;
    char * os_name;
} _ArchInfo;

typedef struct _Arch {
    const char * name;
    int bits;
    const char * basearch;
    const char * compatible_native_arches[MAX_ARCHES];
    const char * compatible_multilib_arches[MAX_ARCHES];
    int is_multilib;
    int is_noarch;
    int is_source;
} _Arch;


static struct _Arch arch_list[] = {
    // basearch: aarch64
    {
        .name = "aarch64",
        .bits = 64,
        .basearch = "aarch64",
        .compatible_native_arches = {"aarch64", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: armhfp
    {
        .name = "armv6hl",
        .bits = 32,
        .basearch = "armhfp",
        .compatible_native_arches = {"armv6hl", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "armv7hl",
        .bits = 32,
        .basearch = "armhfp",
        .compatible_native_arches = {"armv7hl", "armv6hl", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "armv7hnl",
        .bits = 32,
        .basearch = "armhfp",
        .compatible_native_arches = {"armv7hnl", "armv7hl", "armv6hl", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "armv8hl",
        .bits = 32,
        .basearch = "armhfp",
        .compatible_native_arches = {"armv8hl", "armv7hnl", "armv7hl", "armv6hl", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: i386
    {
        .name = "athlon",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"athlon", "i686", "i586", "i486", "i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "geode",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"geode", "i686", "i586", "i486", "i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "i686",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"i686", "i586", "i486", "i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "i586",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"i586", "i486", "i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "i486",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"i486", "i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "i386",
        .bits = 32,
        .basearch = "i386",
        .compatible_native_arches = {"i386", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: ia64
    {
        .name = "ia64",
        .bits = 64,
        .basearch = "ia64",
        .compatible_native_arches = {"ia64", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: ppc
    {
        .name = "ppc",
        .bits = 32,
        .basearch = "ppc",
        .compatible_native_arches = {"ppc", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: ppc64
    {
        .name = "ppc64",
        .bits = 64,
        .basearch = "ppc64",
        .compatible_native_arches = {"ppc64", "noarch", NULL},
        .compatible_multilib_arches = {"ppc", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "ppc64p7",
        .bits = 64,
        .basearch = "ppc64",
        .compatible_native_arches = {"ppc64p7", "ppc64", "noarch", NULL},
        .compatible_multilib_arches = {"ppc", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: ppc64le
    {
        .name = "ppc64le",
        .bits = 64,
        .basearch = "ppc64le",
        .compatible_native_arches = {"ppc64le", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: s390
    {
        .name = "s390",
        .bits = 32,
        .basearch = "s390",
        .compatible_native_arches = {"s390", "noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: s390x
    {
        .name = "s390x",
        .bits = 64,
        .basearch = "s390x",
        .compatible_native_arches = {"s390x", "noarch", NULL},
        .compatible_multilib_arches = {"s390", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: x86_64
    {
        .name = "amd64",
        .bits = 64,
        .basearch = "x86_64",
        .compatible_native_arches = {"amd64", "x86_64", "noarch", NULL},
        .compatible_multilib_arches = {"athlon", "i686", "i586", "i486", "i386", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "ia32e",
        .bits = 64,
        .basearch = "x86_64",
        .compatible_native_arches = {"ia32e", "x86_64", "noarch", NULL},
        .compatible_multilib_arches = {"athlon", "i686", "i586", "i486", "i386", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },
    {
        .name = "x86_64",
        .bits = 64,
        .basearch = "x86_64",
        .compatible_native_arches = {"x86_64", "noarch", NULL},
        .compatible_multilib_arches = {"athlon", "i686", "i586", "i486", "i386", NULL},
        .is_multilib = 1,
        .is_noarch = 0,
        .is_source = 0,
    },

    // basearch: noarch
    {
        .name = "noarch",
        .bits = 0,
        .basearch = "noarch",
        .compatible_native_arches = {"noarch", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 1,
        .is_source = 0,
    },

    // basaearch: src
    {
        .name = "src",
        .bits = 0,
        .basearch = "src",
        .compatible_native_arches = {"src", "nosrc", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 1,
    },
    {
        .name = "nosrc",
        .bits = 0,
        .basearch = "src",
        .compatible_native_arches = {"src", "nosrc", NULL},
        .compatible_multilib_arches = {NULL},
        .is_multilib = 0,
        .is_noarch = 0,
        .is_source = 1,
    },
};


#define ARCH_LIST_LEN (sizeof(arch_list)/sizeof(*arch_list))

typedef struct _Canon {
    const char * name;
    const char * canon_name;
} _Canon;


// needs to be in alphabetical order!
static struct _Canon arch_canon_names[] = {
    { .name = "atariclone", .canon_name = "m68kmint" },
    { .name = "atarist", .canon_name = "m68kmint" },
    { .name = "atariste", .canon_name = "m68kmint" },
    { .name = "ataritt", .canon_name = "m68kmint" },
    { .name = "falcon", .canon_name = "m68kmint" },
    { .name = "hades", .canon_name = "m68kmint" },
    { .name = "milan", .canon_name = "m68kmint" },
    { .name = "riscv", .canon_name = "riscv64" },
};

#define CANON_ARCH_LIST_LEN (sizeof(arch_canon_names)/sizeof(*arch_canon_names))

// needs to be in alphabetical order!
static struct _Canon os_canon_names[] = {
    { .name = "BSD_OS", .canon_name = "bsdi" },
    { .name = "CYGWIN32_95", .canon_name = "cygwin32" },
    { .name = "CYGWIN32_NT", .canon_name = "cygwin32" },
    { .name = "Darwin", .canon_name = "darwin" },
    { .name = "HP-UX", .canon_name = "hpux10" },
    { .name = "IRIX64", .canon_name = "Irix64" },
    { .name = "Linux/390", .canon_name = "OS/390" },
    { .name = "Linux/ESA", .canon_name = "VM/ESA" },
    { .name = "MacOSX", .canon_name = "macosx" },
    { .name = "MiNT", .canon_name = "FreeMiNT" },
    { .name = "NEXTSTEP", .canon_name = "NextStep" },
    { .name = "OSF1", .canon_name = "osf1" },
    { .name = "SCO_SV", .canon_name = "SCO_SV3.2v5.0.2" },
    { .name = "UNIX_SV", .canon_name = "MP_RAS:" },
    { .name = "osf3.2", .canon_name = "osf1" },
    { .name = "osf4.0", .canon_name = "osf1" },
};

#define CANON_OS_LIST_LEN (sizeof(os_canon_names)/sizeof(*os_canon_names))


libarch_archinfo * libarch_init() {
    libarch_archinfo * result = malloc(sizeof(libarch_archinfo));
    result->arch_list = arch_list;
    return result;
}

void libarch_free(libarch_archinfo * archinfo) {
    free(archinfo);
}

const libarch_arch * libarch_get_arch(const libarch_archinfo * archinfo, const char * name) {
    const libarch_arch * result = NULL;
    for (size_t i = 0; i < ARCH_LIST_LEN; i++) {
        if (strcmp(archinfo->arch_list[i].name, name) == 0) {
            result = &(archinfo->arch_list[i]);
            break;
        }
    }
    return result;
}


const char * libarch_arch_get_name(const libarch_arch * arch) {
    return arch->name;
}


const char * libarch_arch_get_basearch(const libarch_arch * arch) {
    return arch->basearch;
}


int libarch_arch_get_bits(const libarch_arch * arch) {
    return arch->bits;
}


const char * const * libarch_arch_get_compatible_native_arches(const libarch_arch * arch) {
    return arch->compatible_native_arches;
}


const char * const * libarch_arch_get_compatible_multilib_arches(const libarch_arch * arch) {
    return arch->compatible_multilib_arches;
}


int libarch_arch_is_multilib(const libarch_arch * arch) {
    return arch->is_multilib;
}


int libarch_arch_is_noarch(const libarch_arch * arch) {
    return arch->is_noarch;
}


int libarch_arch_is_source(const libarch_arch * arch) {
    return arch->is_source;
}


char ** libarch_basearch_get_compatible_native_arches(const char * basearch) {
    // arch_list is a superset of all architectures,
    // allocating by it's length + 1 for the NULL terminator should be sufficient
    char ** result = malloc((ARCH_LIST_LEN + 1) * sizeof(char *));
    result[0] = NULL;


    for (size_t i = 0; i < ARCH_LIST_LEN; i++) {
        const libarch_arch * arch = &arch_list[i];
        if (arch->name == NULL) {
            // ARCH_LIST_LEN is greater than actual array length
            break;
        }
        if (strcmp(arch->basearch, basearch) != 0) {
            // skip architectures with a different basearch
            continue;
        }
        char ** item = (char **)arch->compatible_native_arches;
        while (*item != NULL) {
            if (list_find(result, *item) == -1) {
                list_append(result, *item);
            }
            item++;
        }
    }
    return result;
}


char ** libarch_basearch_get_compatible_multilib_arches(const char * basearch) {
    // arch_list is a superset of all architectures,
    // allocating by it's length + 1 for the NULL terminator should be sufficient
    char ** result = malloc((ARCH_LIST_LEN + 1) * sizeof(char *));
    result[0] = NULL;


    for (size_t i = 0; i < ARCH_LIST_LEN; i++) {
        const libarch_arch * arch = &arch_list[i];
        if (arch->name == NULL) {
            // ARCH_LIST_LEN is greater than actual array length
            break;
        }
        if (strcmp(arch->basearch, basearch) != 0) {
            // skip architectures with a different basearch
            continue;
        }
        char ** item = (char **)arch->compatible_multilib_arches;
        while (*item != NULL) {
            if (list_find(result, *item) == -1) {
                list_append(result, *item);
            }
            item++;
        }
    }
    return result;
}

int cmpentry(const char * name, const struct _Canon * entry){
    return strcmp(name, entry->name);
}

const char * libarch_arch_get_canonical_name(const char * arch) {
    struct _Canon * entry;
    entry = bsearch(arch, arch_canon_names, CANON_ARCH_LIST_LEN,
		    sizeof(struct _Canon),
		    (int (*)(const void *, const void *))&cmpentry);
    if (entry)
	return entry->canon_name;
    else
	return arch;
}

const char * libarch_os_get_canonical_name(const char * os) {
    struct _Canon * entry;
    entry = bsearch(os, os_canon_names, CANON_OS_LIST_LEN,
		    sizeof(struct _Canon),
		    (int (*)(const void *, const void *))&cmpentry);
    if (entry)
	return entry->canon_name;
    else
	return os;
}
