cmake_minimum_required(VERSION 3.13)
project(SeatCraftDesignModel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(
        -Werror -Wall -Wextra -Weffc++ -Wconversion -pedantic -Werror=return-type
    )
endif ()

if (MSVC)
    add_compile_options("/utf-8")
endif (MSVC)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
endif ()


add_files_by_extension(DESIGN_MODEL_SRC_FILES ".h;.hpp;.cpp"
    src
)

add_files_by_extension(DESIGN_MODEL_PUBLIC_HEADER_FILES ".h;.hpp"
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/
)

message(STATUS "DESIGN_MODEL_PUBLIC_HEADER_FILES: ${DESIGN_MODEL_PUBLIC_HEADER_FILES}")
message(STATUS "DESIGN_MODEL_SRC_FILES: ${DESIGN_MODEL_SRC_FILES}")

add_library(${PROJECT_NAME} STATIC ${DESIGN_MODEL_SRC_FILES} ${DESIGN_MODEL_PUBLIC_HEADER_FILES})

add_source_group(DESIGN_MODEL_PUBLIC_HEADER_FILES ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME} "PublicHeaders")
add_source_group(DESIGN_MODEL_SRC_FILES ${CMAKE_CURRENT_LIST_DIR} "Sources")

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} tgfx nlohmann_json::nlohmann_json)
