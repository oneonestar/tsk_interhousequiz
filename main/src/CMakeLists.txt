cmake_minimum_required (VERSION 2.6)

#default database is MYSQL
SET(DB "MYSQL" CACHE STRING "Select Database")

#project info
project(quiz)
SET(MyCSources buzzer.c non_blocking_socket.c quiz_server.c score.c)

#required library
SET(MyLinkedLib event)

#if using MYSQL
if (${DB} STREQUAL "MYSQL")
	message("Using MYSQL.")
	SET(MyCSources ${MyCSources} db_mysql.c)

	#setup MYSQL_CONFIG
	include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_MYSQL.txt)
	SetMYSQL()
	include_directories(${MYSQL_INCLUDE_DIRECTORIES})
	SET_SOURCE_FILES_PROPERTIES( quiz_server.c PROPERTIES COMPILE_FLAGS -DUSE_MYSQL )
	SET(MyLinkedLib ${MyLinkedLib} ${MYSQL_LIBRARY})

#if using Redis
elseif (${DB} STREQUAL "Redis")
	message("Using Redis.")
	SET(MyCSources ${MyCSources} db_redis.c )

	#setup Redis
	add_library(lib_redis STATIC IMPORTED)
	set_property(TARGET lib_redis PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libhiredis.a )
	SET_SOURCE_FILES_PROPERTIES( quiz_server.c PROPERTIES COMPILE_FLAGS -DUSE_REDIS )
	SET(MyLinkedLib ${MyLinkedLib} lib_redis)

#if using Mongodb
elseif (${DB} STREQUAL "Mongodb")
  message("Mongodb")
endif()

ADD_EXECUTABLE(quiz ${MyCSources})

SET(CMAKE_BUILD_TYPE "DEBUG")
SET(CMAKE_C_FLAGS_DEBUG "-Wall")

SET_SOURCE_FILES_PROPERTIES( score.c PROPERTIES COMPILE_FLAGS -std=c99 )
target_link_libraries(quiz ${MyLinkedLib})
