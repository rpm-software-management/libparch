message("Building bindings for perl5: libparch")


foreach(MODULE_NAME ${SWIG_LIBPARCH_MODULES})
    add_perl5_module(libparch ${MODULE_NAME})
endforeach()
