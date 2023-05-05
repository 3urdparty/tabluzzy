
###########
# Library #
###########

cmake_minimum_required(VERSION 3.26.0)
set (CMAKE_CXX_STANDARD 17)

# Setting library name
set(LIBRARY_NAME
    tabluzzy
)

# Setting main environment variables
set(MAIN_LIBRARIES_DIR
    libs
)

set(MAIN_LIBRARIES 
    statsi
    strfmt
    terminalcancer
)

set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)
set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/tabluzzy.hpp
)
set(LIBRARY_SOURCE_DIR
    src
)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/tables.cpp
    ${LIBRARY_SOURCE_DIR}/columns`.cpp
)



# Created project
project(${LIBRARY_NAME})





if (TARGET ${LIBRARY_NAME})
  # Do something when target found
else()
add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)
endif()


#########
# Tests #
#########

#Setting target name for tets
set(TESTS_NAME 
    tabluzzy_tests
)

#Setting directory for test files
set(TESTS_DIR 
    tests
)

#Add test files here
set(TESTS_SOURCES
    ${TESTS_DIR}/test.cpp
)



# Setting up GTest
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()


# Adding test files to target


if (TARGET ${TESTS_NAME})
  # Do something when target found
else()
add_executable(
    ${TESTS_NAME}
    ${TESTS_SOURCES}
)
# Linking target with GTest
target_link_libraries(
    ${TESTS_NAME}
    GTest::gtest_main
    ${LIBRARY_NAME}
)
# Final steps
include(GoogleTest)
gtest_discover_tests(${TESTS_NAME})
endif()


################
# Dependencies #
################

# Git submodules
message("")
message("Finding and downloading submodules for library `${LIBRARY_NAME}`")
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.git)
	# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if (GIT_SUBMODULE)
		message(STATUS "Submodules updating")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		    RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()
  

foreach(LIBRARY ${MAIN_LIBRARIES})
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/libs/${LIBRARY}/CMakeLists.txt")
	message(FATAL_ERROR "The ${LIBRARY} submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
else()
    message (STATUS "Submodule ${LIBRARY} was downloaded and has been found.")
endif()
message("")
endforeach(LIBRARY)



# Addinng all libraries in libs/
foreach(LIBRARY ${MAIN_LIBRARIES})
    add_subdirectory(${MAIN_LIBRARIES_DIR}/${LIBRARY})
endforeach(LIBRARY)

# Linking libraries/dependencies
target_link_libraries(${LIBRARY_NAME} ${MAIN_LIBRARIES})


# adding include/ directories
target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

# adding more include/ directories
target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Sets public header files for target
set_target_properties(${LIBRARY_NAME} PROPERTIES PUBLIC_HEADER ${LIBRARY_HEADERS})


# Installation properties
install(TARGETS ${LIBRARY_NAME} DESTINATION lib/${LIBRARY_NAME}
PUBLIC_HEADER DESTINATION include/${LIBRARY_NAME} )

