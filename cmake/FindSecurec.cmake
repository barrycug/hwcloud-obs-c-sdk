if(SECUREC_STATIC_LIBRARY)
    set(SECUREC_STATIC securec)
endif()

# find the SECUREC include directory
find_path(SECUREC_INCLUDE_DIR securec.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
          /usr/include/
          ${SECUREC_DIR}/include/)

# find the SECUREC library
find_library(SECUREC_LIBRARY
             NAMES ${SECUREC_STATIC} securec
             PATH_SUFFIXES lib64 lib
                    /usr/local
                    /usr/local/lib/
                    /usr
                    ${SECUREC_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set SECUREC_FOUND to TRUE if all listed variables are TRUE

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    libsecurec DEFAULT_MSG SECUREC_LIBRARY SECUREC_INCLUDE_DIR)
mark_as_advanced(SECUREC_INCLUDE_DIR SECUREC_LIBRARY)