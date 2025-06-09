
cmake_minimum_required(VERSION 3.12)

set(FRSPROJ $ENV{FRSPROJ})
message("FRSPROJ: ${FRSPROJ}")

set(FRSTPSRC $ENV{FRSTPSRC})
message("FRSTPSRC: ${FRSTPSRC}")

set(VCPKG_DIR "${FRSTPSRC}/vcpkg")
message("VCPKG_DIR: ${VCPKG_DIR}")

#set(VCPKG_FEATURE_FLAGS "versions") #enable package versioning in manifest
set(VCPKG_INSTALL_OPTIONS "--allow-unsupported") # to allow boost-log for emscripten
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake CACHE STRING "vcpkg toolchain file")

project(app_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME})

add_subdirectory(${FRSPROJ}/imgui_app_base imgui_app_base)
target_include_directories(${PROJECT_NAME} PUBLIC ${FRSPROJ}/imgui_app_base/src)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui_app_base)

target_include_directories(${PROJECT_NAME} PUBLIC ${FRSPROJ})

add_subdirectory(${FRSPROJ}/frs_utility_cpp frs_utility_cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC frs_utility_cpp)

find_package (Boost REQUIRED COMPONENTS filesystem date_time log log_setup)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost Boost::log Boost::log_setup)

if (WASM)
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

