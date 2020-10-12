if(NOT WITH_TESTS)
    return()
endif()


message("Building tests")


# libparch
#add_subdirectory(libparch)

# bindings
#add_subdirectory(go)
#add_subdirectory(perl5)
add_subdirectory(python3)
#add_subdirectory(ruby)
