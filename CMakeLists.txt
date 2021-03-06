cmake_minimum_required(VERSION 3.2)
project(quicksave)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-ggdb -O2 -fsanitize=address -I/usr/local/include/antlr4-runtime -L/usr/local/lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fuse)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared/io_quicksave_libbeans)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rapidjson/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googlemock/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c/librabbitmq)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c-utils)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sha1)

#TODO
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/PrivacyGuard)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/abseil)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/cctz/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/io_quicksave_qsql/cppqsql/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/io_quicksave_qsql/cppqsql/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/io_quicksave_libbeans/cppbeans/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/libmemcached)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11/deps/date/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11-connector-sqlite3/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11-connector-postgresql/include)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11-connector-sqlite3)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/sqlpp11-connector-postgresql)

include_directories(/usr/include/python3.6m)
include_directories(/usr/local/include/python3.6m)
include_directories(/usr/include/antlr4-runtime)

SET(CORE_LIBRARIES dl util pthread)
SET(BASE_LIBRARIES glog gflags folly abseil)
SET(DBMS_LIBRARIES sqlpp11-connector-postgresql pq sqlpp11-connector-sqlite3 sqlite3 sqlpp11)
SET(HTTP_LIBRARIES proxygenhttpserver )

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c-utils)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/sha1)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/libmemcached)

ADD_DEFINITIONS(-DSQLITE_THREADSAFE=1 -Wl,--export-dynamic)


FILE(GLOB_RECURSE PROJECT_QSQL ${CMAKE_CURRENT_SOURCE_DIR}/deps/io_quicksave_qsql/cppqsql/generated/qsql/**.cpp)
ADD_LIBRARY(qsql ${PROJECT_QSQL})
TARGET_LINK_LIBRARIES(qsql dl antlr4-runtime)

FILE(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/api/**.cc)
FILE(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/source/**.h)
ADD_EXECUTABLE(api
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS})
TARGET_LINK_LIBRARIES(api
        qsql antlr4-runtime
        libpython3.6m.so
        memcached
        sha1
        stdc++fs
        uuid
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

ADD_EXECUTABLE(cdn cdn/cdn.cc)
TARGET_LINK_LIBRARIES(cdn
        memcached
        stdc++fs
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

ADD_EXECUTABLE(oauth oauth/oauth.cc)
TARGET_LINK_LIBRARIES(oauth
        memcached
        uuid
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

ADD_EXECUTABLE(post post/post.cc)
TARGET_LINK_LIBRARIES(post
        rabbitmq-static
        rabbitmq-c-utils
        sha1
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

FILE(GLOB_RECURSE UNIT_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/**.cc)
ADD_EXECUTABLE(unit_test ${UNIT_TESTS})
TARGET_LINK_LIBRARIES(unit_test ${PROJECT_LIBRARIES} python3.6m sha1 antlr4-runtime qsql
        gtest gmock
        curl
        uuid
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )


ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)
FILE(GLOB_RECURSE FUSE_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/fuse/**.cc)
ADD_EXECUTABLE(qsfuse ${FUSE_MAIN})
TARGET_LINK_LIBRARIES(qsfuse
        fuse
        qsql antlr4-runtime
        libpython3.6m.so
        memcached
        sha1
        stdc++fs
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

FILE(GLOB_RECURSE FUSE_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/fuse_test/**.cc)
FILE(GLOB_RECURSE FUSE_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/fuse/**.h)
ADD_EXECUTABLE(fuse_test ${FUSE_TESTS} ${FUSE_HEAD})
TARGET_LINK_LIBRARIES(fuse_test ${PROJECT_LIBRARIES}
        fuse
        gtest gmock
        qsql antlr4-runtime
        libpython3.6m.so
        memcached
        sha1
        stdc++fs
        ${CORE_LIBRARIES}
        ${BASE_LIBRARIES}
        ${DBMS_LIBRARIES}
        ${HTTP_LIBRARIES}
        )

