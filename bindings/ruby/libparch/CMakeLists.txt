message("Building bindings for ruby: libparch")


foreach(MODULE_NAME ${SWIG_LIBPARCH_MODULES})
    add_ruby_module(libparch ${MODULE_NAME})
endforeach()
