#ifndef LIBPARCH_ARCH_H
#define LIBPARCH_ARCH_H


typedef struct _Arch libparch_arch;
typedef struct _ArchInfo libparch_archinfo;

/// Return empty archinfo instance
// struct libparch_archinfo * libparch_new();

/// Return archinfo instance with the default information
struct libparch_archinfo * libparch_init();

/// Free archinfo
void libparch_free(struct libparch_archinfo * archinfo);

/// Return pointer to Arch or NULL if not found.
///
/// Data ownership: The data is static and MUST NOT be freed.
const struct libparch_arch * libparch_get_arch(const struct libparch_archinfo * archinfo, const char * name);

/// Detect arch and OS of the current machine
const struct libparch_arch * libparch_detected_arch(const struct libparch_archinfo * archinfo);
const char * libparch_detected_arch_name(const struct libparch_archinfo * archinfo);
const char * libparch_detected_os(const struct libparch_archinfo * archinfo);

/// Get canonical names from (possibly more specific) name
/// returned from libparch_autodetect_*
const char * libparch_arch_get_canonical_name(const char * os);
const char * libparch_os_get_canonical_name(const char * os);




/// Return name of an Arch.
//
/// Data ownership: The data is static and MUST NOT be freed.
const char * libparch_arch_get_name(const struct libparch_arch * arch);


/// Return basearch of an Arch.
//
/// Data ownership: The data is static and MUST NOT be freed.
const char * libparch_arch_get_basearch(const struct libparch_arch * arch);


/// Return number of bits of an Arch.
/// Typical values are 64 or 32.
/// If 0 is returned, then the architecture is either noarch or source.
///
/// Data ownership: The data is static and MUST NOT be freed.
int libparch_arch_get_bits(const struct libparch_arch * arch);


const char *const * libparch_arch_get_compatible_native_arches(const struct libparch_arch * arch);
const char * const * libparch_arch_get_compatible_multilib_arches(const struct libparch_arch * arch);


/// Determine if an Arch is multilib.
int libparch_arch_is_multilib(const struct libparch_arch * arch);


/// Determine if an Arch is noarch.
int libparch_arch_is_noarch(const struct libparch_arch * arch);


/// Determine if an Arch is source.
int libparch_arch_is_source(const struct libparch_arch * arch);

#endif  // LIBPARCH_ARCH_H
