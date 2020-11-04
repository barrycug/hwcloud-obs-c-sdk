if(ESDKLOGAPI_STATIC_LIBRARY)
    set(ESDKLOGAPI_STATIC eSDKLogAPI)
endif()

# find the ESDKLOGAPI include directory
find_path(ESDKLOGAPI_INCLUDE_DIR eSDKLogAPI.h
          PATH_SUFFIXES include
          PATHS
          /usr/local/include/
          /usr/include/
          ${ESDKLOGAPI_DIR}/include/)

# find the ESDKLOGAPI library
find_library(ESDKLOGAPI_LIBRARY
             NAMES ${ESDKLOGAPI_STATIC} eSDKLogAPI
             PATH_SUFFIXES lib64 lib
                    /usr/local
                    /usr/local/lib/
                    /usr
                    ${ESDKLOGAPI_DIR}/lib)

# handle the QUIETLY and REQUIRED arguments and set ESDKLOGAPI_FOUND to TRUE if all listed variables are TRUE

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    libeSDKLogAPI DEFAULT_MSG ESDKLOGAPI_LIBRARY ESDKLOGAPI_INCLUDE_DIR)
mark_as_advanced(ESDKLOGAPI_INCLUDE_DIR ESDKLOGAPI_LIBRARY)