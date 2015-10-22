cmake_minimum_required (VERSION 2.8)
include(ExternalProject)

project (odeint_appveyor)
set(CMAKE_VERBOSE_MAKEFILE 1)
#enable_testing()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_EXECUTABLE(catch.exe main_catch.cpp)

# for header-only git http://stackoverflow.com/a/21223763
ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    GIT_TAG v1.2.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_INSTALL ON
    )
ExternalProject_Get_Property(catch source_dir)
set(CatchIncludeDir ${source_dir}/single_include)

if(WIN32)
    set(Boost_USE_STATIC_LIBS ON) # only find static libs (this was crucial before that nothing worked on Windows, screws things up for Debian though)
    set(BOOST_ROOT "C:/Libraries/boost_1_59_0")
    message(WIN32 BOOST_LIBRARYDIR  " " ${BOOST_LIBRARYDIR})
    message(WIN32 BOOST_ROOT " " ${BOOST_ROOT})
endif()

add_dependencies(catch.exe catch)
target_include_directories(catch.exe PRIVATE ${CatchIncludeDir})
