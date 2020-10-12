message("Building bindings for go: libparch")


foreach(MODULE_NAME ${SWIG_LIBPARCH_MODULES})
    add_go_module(libparch ${MODULE_NAME})
endforeach()
