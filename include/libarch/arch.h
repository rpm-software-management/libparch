#ifndef LIBARCH_ARCH_H
#define LIBARCH_ARCH_H


typedef struct _Arch libarch_arch;
typedef struct _ArchInfo libarch_archinfo;

/// Return empty archinfo instance
// libarch_archinfo * libarch_new();

/// Return archinfo instance with the default information
libarch_archinfo * libarch_init();

/// Free archinfo
void libarch_free(libarch_archinfo * archinfo);

/// Return pointer to Arch or NULL if not found.
///
/// Data ownership: The data is static and MUST NOT be freed.
const libarch_arch * libarch_get_arch(const libarch_archinfo * archinfo, const char * name);

/// Detect arch and OS of the current machine
const libarch_arch * libarch_detected_arch(const libarch_archinfo * archinfo);
const char * libarch_detected_arch_name(const libarch_archinfo * archinfo);
const char * libarch_detected_os(const libarch_archinfo * archinfo);

/// Get canonical names from (possibly more specific) name
/// returned from libarch_autodetect_*
const char * libarch_arch_get_canonical_name(const char * os);
const char * libarch_os_get_canonical_name(const char * os);




/// Return name of an Arch.
//
/// Data ownership: The data is static and MUST NOT be freed.
const char * libarch_arch_get_name(const libarch_arch * arch);


/// Return basearch of an Arch.
//
/// Data ownership: The data is static and MUST NOT be freed.
const char * libarch_arch_get_basearch(const libarch_arch * arch);


/// Return number of bits of an Arch.
/// Typical values are 64 or 32.
/// If 0 is returned, then the architecture is either noarch or source.
///
/// Data ownership: The data is static and MUST NOT be freed.
int libarch_arch_get_bits(const libarch_arch * arch);


const char *const * libarch_arch_get_compatible_native_arches(const libarch_arch * arch);
const char * const * libarch_arch_get_compatible_multilib_arches(const libarch_arch * arch);


/// Determine if an Arch is multilib.
int libarch_arch_is_multilib(const libarch_arch * arch);


/// Determine if an Arch is noarch.
int libarch_arch_is_noarch(const libarch_arch * arch);


/// Determine if an Arch is source.
int libarch_arch_is_source(const libarch_arch * arch);

#endif  // LIBARCH_ARCH_H
