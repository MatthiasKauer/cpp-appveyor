cmake_minimum_required (VERSION 2.8)
include(ExternalProject)

project (odeint_appveyor)
set(CMAKE_VERBOSE_MAKEFILE 1)
#enable_testing()

# for header-only git http://stackoverflow.com/a/21223763
ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    GIT_TAG v1.2.1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
        <SOURCE_DIR>/single_include ${CMAKE_BINARY_DIR}/installed/catch
    LOG_DOWNLOAD ON
    LOG_INSTALL ON
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



