# use any sources found under the current directory
file(GLOB_RECURSE LIBPARCH_SOURCES *.c)

# check headers and libraries
include(CheckIncludeFiles)

check_include_files(stdio.h HAVE_STDIO_H)
check_include_files(string.h HAVE_STRING_H)

check_include_files(sys/utsname.h HAVE_SYS_UTSNAME_H)
check_include_files(sys/systemcfg.h HAVE_SYS_SYSTEMCFG_H)


configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# build libparch.so
add_library(libparch SHARED ${LIBPARCH_SOURCES})
set_target_properties(libparch PROPERTIES LINKER_LANGUAGE C)
set_target_properties(libparch PROPERTIES OUTPUT_NAME "parch")
set_target_properties(libparch PROPERTIES SOVERSION 0)


# install libparch.so
install(TARGETS libparch LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})


# create a .pc file
configure_file("libparch.pc.in" ${CMAKE_CURRENT_BINARY_DIR}/libparch.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libparch.pc DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig)
