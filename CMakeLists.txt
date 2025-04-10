cmake_minimum_required(VERSION 3.27)

project(SHARPSAT VERSION 13.03)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wno-error=narrowing)

option(BUILD_SHARED_LIBS "Link to shared libraries." ON)

if(NOT BUILD_SHARED_LIBS)
    set(GMP_USE_STATIC_LIBS ON)
endif()

find_package(GMP REQUIRED)

add_library(component_types OBJECT
    src/component_types/base_packed_component.cpp
    src/component_types/component_archetype.cpp
)

add_library(libsharpSAT STATIC
    $<TARGET_OBJECTS:component_types>
    src/alt_component_analyzer.cpp
    src/component_analyzer.cpp
    src/component_cache.cpp
    src/component_management.cpp
    src/instance.cpp
    src/new_component_analyzer.cpp
    src/solver.cpp
    src/statistics.cpp
)

target_link_libraries(libsharpSAT GMP::GMPXX GMP::GMP)

add_executable(sharpSAT src/main.cpp)
target_link_libraries(sharpSAT libsharpSAT)

install(TARGETS sharpSAT)
