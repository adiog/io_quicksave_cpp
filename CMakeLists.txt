cmake_minimum_required(VERSION 3.2)
project(quicksave)

#set(CMAKE_CXX_COMPILER /usr/bin/g++-5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-ggdb -O0 -fsanitize=address -I/usr/local/include/antlr4-runtime -L/usr/local/lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/generated)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fuse)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared/libbeans)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/shared/plugin-engine)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rapidjson/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/beans/cppbeans/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest/googlemock/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/SQLiteCpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c/librabbitmq)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c-utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/sha1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/PrivacyGuard)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/postgres/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/abseil)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/cctz/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/qsql/cppqsql/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/qsql/cppqsql/generated)

include_directories(/usr/include/python3.6m)
include_directories(/usr/local/include/python3.6m)
include_directories(/usr/include/antlr4-runtime)

SET(PROJECT_LIBRARIES proxygenhttpserver folly glog gflags util abseil)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/googletest)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/SQLiteCpp)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/rabbitmq-c-utils)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/sha1)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/deps/postgres)

ADD_DEFINITIONS(-DSQLITE_THREADSAFE=1 -Wl,--export-dynamic)


FILE(GLOB_RECURSE PROJECT_QSQL ${CMAKE_CURRENT_SOURCE_DIR}/deps/qsql/cppqsql/generated/qsql/**.cpp)
ADD_LIBRARY(qsql ${PROJECT_QSQL})
TARGET_LINK_LIBRARIES(qsql dl antlr4-runtime)

FILE(GLOB_RECURSE PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/api/**.cc)
FILE(GLOB_RECURSE PROJECT_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/source/**.h)
ADD_EXECUTABLE(api ${PROJECT_MAIN} ${PROJECT_HEAD})
TARGET_LINK_LIBRARIES(api ${PROJECT_LIBRARIES} postgres pq libpython3.6m.so memcached rabbitmq-c-utils rabbitmq-static ssl SQLiteCpp sqlite3 antlr4-runtime qsql pthread sha1 stdc++fs dl util)

ADD_EXECUTABLE(cdn cdn/cdn.cc)
TARGET_LINK_LIBRARIES(cdn ${PROJECT_LIBRARIES} postgres pq memcached pthread SQLiteCpp sqlite3 stdc++fs dl)

ADD_EXECUTABLE(oauth oauth/oauth.cc)
TARGET_LINK_LIBRARIES(oauth ${PROJECT_LIBRARIES} postgres pq memcached pthread SQLiteCpp sqlite3 dl)

ADD_EXECUTABLE(post post/post.cc)
TARGET_LINK_LIBRARIES(post pthread glog gflags rabbitmq-static rabbitmq-c-utils sha1 postgres pq SQLiteCpp sqlite3 dl folly abseil)

FILE(GLOB_RECURSE UNIT_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/unit_test/**.cc)
ADD_EXECUTABLE(unit_test ${UNIT_TESTS})
TARGET_LINK_LIBRARIES(unit_test ${PROJECT_LIBRARIES} python3.6m sha1 antlr4-runtime qsql gtest gmock pthread SQLiteCpp sqlite3 dl dl)


ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64)
FILE(GLOB_RECURSE FUSE_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/fuse/**.cc)
ADD_EXECUTABLE(qsfuse ${FUSE_MAIN})
TARGET_LINK_LIBRARIES(qsfuse ${PROJECT_LIBRARIES} python3.6m postgres pq sha1 antlr4-runtime qsql gtest gmock pthread SQLiteCpp sqlite3 fuse stdc++fs dl)

FILE(GLOB_RECURSE FUSE_TESTS ${CMAKE_CURRENT_SOURCE_DIR}/fuse_test/**.cc)
FILE(GLOB_RECURSE FUSE_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/fuse/**.h)
ADD_EXECUTABLE(fuse_test ${FUSE_TESTS} ${FUSE_HEAD})
TARGET_LINK_LIBRARIES(fuse_test ${PROJECT_LIBRARIES} python3.6m postgres pq sha1 antlr4-runtime qsql gtest gmock pthread SQLiteCpp sqlite3 stdc++fs dl)

