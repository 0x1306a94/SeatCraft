cmake_minimum_required(VERSION 3.13)
project(SeatCraftEditorCore)

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

if (APPLE)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices REQUIRED)
    list(APPEND ${PROJECT_NAME}_LIBS  ${APPLICATION_SERVICES_FRAMEWORK})
    find_library(QUARTZ_CORE QuartzCore REQUIRED)
    list(APPEND ${PROJECT_NAME}_LIBS  ${QUARTZ_CORE})
    find_library(COCOA Cocoa REQUIRED)
    list(APPEND ${PROJECT_NAME}_LIBS  ${COCOA})
    find_library(FOUNDATION Foundation REQUIRED)
    list(APPEND ${PROJECT_NAME}_LIBS  ${FOUNDATION})
    find_library(ICONV_LIBRARIES NAMES iconv libiconv libiconv-2 c)
    list(APPEND ${PROJECT_NAME}_LIBS  ${ICONV_LIBRARIES})
    find_library(VIDEOTOOLBOX VideoToolbox)
    list(APPEND ${PROJECT_NAME}_LIBS  ${VIDEOTOOLBOX})
    find_library(CORE_MEDIA CoreMedia)
    list(APPEND ${PROJECT_NAME}_LIBS  ${CORE_MEDIA})
    find_library(COMPRESSION_LIBRARIES NAMES compression)
    list(APPEND ${PROJECT_NAME}_LIBS  ${COMPRESSION_LIBRARIES})
elseif (WIN32)
    set(BUILD_USE_64BITS ON)
    add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
    find_library(Bcrypt_LIB Bcrypt)
    list(APPEND ${PROJECT_NAME}_LIBS  ${Bcrypt_LIB})
    find_library(ws2_32_LIB ws2_32)
    list(APPEND ${PROJECT_NAME}_LIBS  ${ws2_32_LIB})
endif()

add_files_by_extension(EDITOR_CORE_SRC_FILES ".h;.hpp;.cpp"
    renderer
    drawers
    app
)

add_files_by_extension(EDITOR_CORE_PUBLIC_HEADER_FILES ".h;.hpp"
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/app
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/common
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/renderer
)

message(STATUS "EDITOR_CORE_PUBLIC_HEADER_FILES: ${EDITOR_CORE_PUBLIC_HEADER_FILES}")
message(STATUS "EDITOR_CORE_SRC_FILES: ${EDITOR_CORE_SRC_FILES}")

add_library(${PROJECT_NAME} STATIC ${EDITOR_CORE_SRC_FILES} ${EDITOR_CORE_PUBLIC_HEADER_FILES})

add_source_group(EDITOR_CORE_PUBLIC_HEADER_FILES ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME} "PublicHeaders")
add_source_group(EDITOR_CORE_SRC_FILES ${CMAKE_CURRENT_LIST_DIR} "Sources")

target_include_directories(${PROJECT_NAME} PRIVATE
    .
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    ${${PROJECT_NAME}_LIBS}
    tgfx
)
