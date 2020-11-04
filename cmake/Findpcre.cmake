if(PCRE_STATIC_LIBRARY)
    set(PCRE_STATIC pcre)
endif()

# find the PCRE include directory
find_path(PCRE_INCLUDE_DIR pcre.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
          /usr/include/
          ${PCRE_DIR}/include/)

# find the PCRE library
find_library(PCRE_LIBRARY
             NAMES ${PCRE_STATIC} pcre
             PATH_SUFFIXES lib64 lib
                    /usr/local
                    /usr/local/lib/
                    /usr
                    ${PCRE_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set PCRE_FOUND to TRUE if all listed variables are TRUE

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    libpcre DEFAULT_MSG PCRE_LIBRARY PCRE_INCLUDE_DIR)
mark_as_advanced(PCRE_INCLUDE_DIR PCRE_LIBRARY)