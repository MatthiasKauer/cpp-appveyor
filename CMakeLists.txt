cmake_minimum_required (VERSION 2.8)
include(ExternalProject)

project (odeint_appveyor)
set(CMAKE_VERBOSE_MAKEFILE 1)
#enable_testing()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ADD_EXECUTABLE(test_runner main_catch.cpp)

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

if(WIN32)   #true for all Windows (not Cygwin!) https://cmake.org/Wiki/CMake_Useful_Variables
    #YOU CURRENTLY MUST NOT COMPILE (ALL ARE HEADER ONLY);
    #see guide here: http://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary
    #for me, it was sufficient (essentially what I read there; takes 30-60min): 
    #cmd.exe -> boost folder;
    #bootstrap
    #.\b2


    set(Boost_USE_STATIC_LIBS ON) # only find static libs (this was crucial before that nothing worked on Windows, screws things up for Debian though)
    set(BOOST_ROOT "C:/Libraries/boost_1_59_0")
    message(WIN32 BOOST_LIBRARYDIR  " " ${BOOST_LIBRARYDIR})
    message(WIN32 BOOST_ROOT " " ${BOOST_ROOT})
endif()

find_package(BOOST 1.55)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_dependencies(test_runner catch)
target_include_directories(test_runner PRIVATE ${CatchIncludeDir})
