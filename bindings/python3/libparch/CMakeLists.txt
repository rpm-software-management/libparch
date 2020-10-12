message("Building bindings for python3: libparch")


file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py)
foreach(MODULE_NAME ${SWIG_LIBPARCH_MODULES})
    add_python3_module(libparch ${MODULE_NAME})
    file(APPEND ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "from . import ${MODULE_NAME}\n")
endforeach()


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION ${Python3_SITEARCH}/libparch)
