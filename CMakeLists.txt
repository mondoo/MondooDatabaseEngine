cmake_minimum_required(VERSION 3.1)

project(MondooDBEngine LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(FMT_DIR ${VENDOR_DIR}/fmt)
set(SQLITE_DIR ${VENDOR_DIR}/SQLite)

execute_process(COMMAND "GenerateList.bat" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles)

include(CMakeFiles/sourcelist.in)
add_executable(${PROJECT_NAME} ${sources})

add_library(sqlite3 STATIC
	${SQLITE_DIR}/shell.c
	${SQLITE_DIR}/sqlite3.c
	${SQLITE_DIR}/sqlite3.h
	${SQLITE_DIR}/sqlite3ext.h
	${SQLITE_DIR}/sqlite3.def
)
set_target_properties(sqlite3 PROPERTIES LINKER_LANGUAGE C)
target_link_libraries (${PROJECT_NAME} ${SQLITE_DIR}/sqlite3.lib)

add_subdirectory(vendor/fmt)
find_package(fmt)
target_link_libraries (${PROJECT_NAME} fmt::fmt)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	PRIVATE
		${SQLITE_DIR} sqlite3
		${FMT_DIR}/include fmt
		${VENDOR_DIR}/reflection reflection
		src/public
		src/private
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	#DEBUG
	COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_DIR}/sqlite3.dll ${CMAKE_BINARY_DIR}
	#RELEASE
	COMMAND ${CMAKE_COMMAND} -E copy ${SQLITE_DIR}/sqlite3.dll ${CMAKE_BINARY_DIR}/Release
)
