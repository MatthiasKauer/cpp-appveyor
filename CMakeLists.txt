cmake_minimum_required (VERSION 2.6)
project (battery_lib_cpp)
set(CMAKE_VERBOSE_MAKEFILE 1)
#enable_testing()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#YOU MUST SET BOOST_ROOT (either as environment variable or in the cmake GUI or HERE)
set(Boost_DEBUG ON)
if(WIN32)
    #YOU MUST COMPILE; see guide here: http://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html#prepare-to-use-a-boost-library-binary
    #for me, it was sufficient (essentially what I read there; takes 30-60min): 
    #cmd.exe -> boost folder;
    #bootstrap
    #.\b2
    SET( Boost_NO_SYSTEM_PATHS true )
    set(BOOST_ROOT "C:/opt/boost_1_55_0") # boost root dir
    set(Boost_USE_STATIC_LIBS ON) # only find static libs (this was crucial before that nothing worked on Windows, screws things up for Debian though)
    message(WIN32 BOOST_LIBRARYDIR  " " ${BOOST_LIBRARYDIR})
    message(WIN32 BOOST_ROOT " " ${BOOST_ROOT})
endif()

#http://stackoverflow.com/questions/6646405/how-do-you-add-boost-libraries-in-cmakelists-txt
#add_definitions( -DBOOST_ALL_NO_LIB )
#add_definitions( -DBOOST_ALL_DYN_LINK )
find_package(Boost 1.50 COMPONENTS timer system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories( ${Boost_LIBRARY_DIRS} )  

set(SRC_FILE src/EquivalentCircuit.cpp src/EquivalentCircuit.h src/CalculationHelper.h src/CalculationHelper.cpp 
    src/TransferParam.cpp src/TransferParam.h src/DataHelper.cpp src/DataHelper.h src/EnumDeclaration.h src/InferredTransferParam.h
        src/ChargeTransferEquationParam.h src/BaseTransferParam.h)
#add_library(battery_lib src/EquivalentCircuit.cpp src/TransferParam.cpp src/DataHelper.cpp )
add_library(battery_lib ${SRC_FILE}) 

#add_executable(battery_lib_run src/EquivalentCircuit.cpp src/main.cpp src/TransferParam.cpp src/DataHelper.cpp )

add_executable(battery_lib_run src/main.cpp)
#http://www.cmake.org/pipermail/cmake/2011-October/047074.html
#target_link_libraries(battery_lib_run battery_lib )
target_link_libraries(battery_lib_run ${Boost_SYSTEM_LIBRARY} ${Boost_TIMER_LIBRARY} battery_lib )


add_executable(catch_test test/EquivalentCircuitTest.cpp)
#target_link_libraries(catch_test battery_lib)
target_link_libraries(catch_test ${Boost_SYSTEM_LIBRARY} ${Boost_TIMER_LIBRARY} battery_lib)

#add_test(MyTest catch_test)
#running tests as part of build
#http://www.cmake.org/pipermail/cmake/2010-January/034420.html
#add_custom_command(TARGET catch_test POST_BUILD COMMAND catch_test)

#look for SWIG
FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS "-Wall")

#look for Pythonlibs
FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET_SOURCE_FILES_PROPERTIES(src/EquivalentCircuit.i PROPERTIES CPLUSPLUS ON)
SET_PROPERTY(SOURCE src/EquivalentCircuit.i PROPERTY SWIG_FLAGS "-builtin")
 
#SWIG_ADD_MODULE(EquivalentCircuit python EquivalentCircuit.i EquivalentCircuit.cpp TransferParam.cpp DataHelper.cpp)
SWIG_ADD_MODULE(EquivalentCircuit python src/EquivalentCircuit.i ${SRC_FILE})
#target_link_libraries(EquivalentCircuit battery_lib)
SWIG_LINK_LIBRARIES(EquivalentCircuit battery_lib ${Boost_SYSTEM_LIBRARY} ${Boost_TIMER_LIBRARY} ${PYTHON_LIBRARIES})

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
COMMENT "Generating API documentation with Doxygen" VERBATIM
)
endif(DOXYGEN_FOUND)
