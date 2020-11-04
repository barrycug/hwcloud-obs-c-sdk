cmake_minimum_required(VERSION 3.8)
SET(CMAKE_BUILD_TYPE "Debug")  
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") 
#option(BUILD_SHARED_LIBS "Build shared libraries" ON)

project(obs-c-sdk)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(REVERSE CMAKE_MODULE_PATH)


file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@1.1/1.1.1g)
list(REVERSE OPENSSL_ROOT_DIR)
find_package(OpenSSL  REQUIRED)
find_package(curl  REQUIRED)
find_package(libXml2  REQUIRED)
find_package(Iconv  REQUIRED)
find_package(pcre)
find_package(securec  REQUIRED)
find_package(eSDKLogAPI  REQUIRED)
set(CMAKE_CXX_STANDARD 14)
list(REVERSE CMAKE_CXX_STANDARD)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall") 
set(INCLUDE_DIR "inc")
set(huaweisecurec_INCLUDE_DIR "inc")

message ( "-- ICONV_INCLUDE_DIR : " ${Iconv_INCLUDE_DIR} )
message ( "-- ESDKLOGAPI_LIBRARY : " ${ESDKLOGAPI_LIBRARY} )

add_library(${PROJECT_NAME} SHARED
                                src/bucket.c
                                src/error_parser.c
                                src/general.c
                                src/object.c
                                src/log.c
                                src/request_context.c
                                src/request_util.c
                                src/request.c
                                src/response_headers_handler.c
                                src/simplexml.c
                                src/util.c
                                )
file(GLOB COMMON_LIBRARIES  ${OPENSSL_LIBRARIES}
                                ${LIBXML2_LIBRARIES}
                                ${CURL_LIBRARY}
                                ${Iconv_LIBRARIES}
                                ${SECUREC_LIBRARY}
                                ${PCRE_LIBRARY}
                                ${ESDKLOGAPI_LIBRARY}
                                
     )
file(GLOB COMMON_INCLUDES ${INCLUDE_DIR}
                          ${OPENSSL_INCLUDE_DIR}
                          ${SECUREC_INCLUDE_DIR}
                          ${LIBXML2_INCLUDE_DIR}
                            )
target_include_directories(${PROJECT_NAME} PUBLIC ${COMMON_INCLUDES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_link_libraries(${PROJECT_NAME}
                                        ${COMMON_LIBRARIES} 
                                        ) 
INSTALL (
  DIRECTORY inc/
  DESTINATION include
  )

INSTALL (
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  )


