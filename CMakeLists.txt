cmake_minimum_required(VERSION 3.12)
project(FinalProject)

set(CMAKE_CXX_STANDARD 14)
set(BOOST_ROOT "G:/boost_1_66_0/")
set(BOOSTROOT "G:/boost_1_66_0/")
set(Boost_INCLUDE_DIR "G:/boost_1_66_0/")
set(BOOST_INCLUDERDIR "G:/boost_1_66_0/")
set(Boost_LIBRARY_DIR "G:/boost_1_66_0/stage/lib")
set(BOOST_LIBRARYDIR "G:/boost_1_66_0/stage/lib")
set(Boost_DEBUG ON)

set(Boost_ARCHITECTURE "-x32")
FIND_PACKAGE(Boost REQUIRED)
IF(${Boost_FOUND})
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    message("find boost!")
ENDIF()

set(SOURCE_FILES main.cpp)
add_executable(FinalProject ${SOURCE_FILES})

include_directories(BackTester)
add_subdirectory(BackTester)
add_subdirectory(Unit_Test)

target_link_libraries(FinalProject BackTester ${Boost_LIBRARIES})
