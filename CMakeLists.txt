cmake_minimum_required(VERSION 3.4.0 FATAL_ERROR)

project(
  Smeagle
  VERSION 1.0
  LANGUAGES CXX
)

set (CMAKE_MODULE_PATH "${Dyninst_DIR}" "${Dyninst_DIR}/Modules" ${CMAKE_MODULE_PATH})
find_package(Dyninst REQUIRED)
find_package(Boost REQUIRED)

set(sources
	source/detail/parser/aarch64.cpp
	source/detail/parser/ppc64le.cpp
	source/detail/parser/x86_64.cpp
	source/corpus.cpp
	source/smeagle.cpp
)

add_library(Smeagle SHARED ${sources})
set_target_properties(Smeagle PROPERTIES CXX_STANDARD 17)
target_link_libraries(Smeagle PRIVATE symtabAPI common instructionAPI)
target_include_directories(Smeagle PRIVATE ${DYNINST_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
target_include_directories(Smeagle PRIVATE source/detail source/detail/parser)
target_include_directories(Smeagle PRIVATE "include")


add_library(Test SHARED test.cpp)
target_compile_options(Test PRIVATE "-g")

add_executable(run run.cpp)
target_link_libraries(run PRIVATE Smeagle)
target_include_directories(run PRIVATE "include")