cmake_minimum_required(VERSION 3.13)
project(SeatCraftEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(
        -Werror -Wall -Wextra -Weffc++ -Wconversion -pedantic -Werror=return-type
        -Wno-gnu-zero-variadic-macro-arguments
    )
endif ()

if (MSVC)
    add_compile_options("/utf-8")
endif (MSVC)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
endif ()

add_definitions(-DQT_NO_KEYWORDS)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets OpenGL Qml Quick)
list(APPEND ${PROJECT_NAME}_LIBS
    tgfx
    Qt${QT_VERSION_MAJOR}::Core 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL 
    Qt${QT_VERSION_MAJOR}::Qml 
    Qt${QT_VERSION_MAJOR}::Quick)

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

qt_standard_project_setup(REQUIRES 6.9)

add_files_by_extension(EDITOR_SRC_FILES ".h;.hpp;.cpp"
    src
    src/view
    src/renderer
)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(${PROJECT_NAME}
    ${EDITOR_SRC_FILES}
)

target_link_libraries(SeatCraftEditor PRIVATE Qt6::Quick)

add_files_by_extension(EDITOR_QML_FILES ".qml"
    assets/qml
)

make_files_relative(EDITOR_QML_FILES_REL ${CMAKE_CURRENT_LIST_DIR} ${EDITOR_QML_FILES})
message(STATUS "EDITOR_QML_FILES_REL: ${EDITOR_QML_FILES_REL}")

qt_add_qml_module(${PROJECT_NAME}
    URI main
    VERSION 1.0
    QML_FILES ${EDITOR_QML_FILES_REL}
)

add_source_group(EDITOR_SRC_FILES ${CMAKE_CURRENT_LIST_DIR}/src "Sources")
add_source_group(EDITOR_QML_FILES ${CMAKE_CURRENT_LIST_DIR}/assets "Assets")

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ./src
    ${SEAT_CRAFT_ROOT_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    SeatCraftEditorCore
    ${${PROJECT_NAME}_LIBS}
)
