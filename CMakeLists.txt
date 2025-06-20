
cmake_minimum_required(VERSION 3.12)

set(FRSTPSRC $ENV{FRSTPSRC})
message("FRSTPSRC: ${FRSTPSRC}")

set(VCPKG_DIR "${FRSTPSRC}/vcpkg")
message("VCPKG_DIR: ${VCPKG_DIR}")

#set(VCPKG_FEATURE_FLAGS "versions") #enable package versioning in manifest
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake CACHE STRING "vcpkg toolchain file")

project(imgui_app_base)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME})

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)

find_package(implot CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC implot::implot)

if (NOT WASM)
	#target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/../..")
	#target_include_directories(${PROJECT_NAME} PUBLIC "/data/proj")

	#add_subdirectory(../../FRS_Utility_CPP FRS_Utility_CPP)
	#target_link_libraries(${PROJECT_NAME} PUBLIC FRS_Utility_CPP)

	#find_package (Boost REQUIRED COMPONENTS filesystem date_time)
	#target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

	find_package(SDL3 CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

	find_package(X11 REQUIRED)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${X11_LIBRARIES})

else()
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=3")
endif()

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

	source_group(" " REGULAR_EXPRESSION ".*")
	source_group("pch" REGULAR_EXPRESSION "cmake_pch\..*")

	target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

source_group(" " REGULAR_EXPRESSION ".*")

file(GLOB_RECURSE SRC_HDR src/*.h)
file(GLOB_RECURSE SRC_CPP src/*.cpp)

target_sources(${PROJECT_NAME} PRIVATE ${SRC_HDR} ${SRC_CPP})

