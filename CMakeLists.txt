cmake_minimum_required(VERSION 3.13)
project(SeatCraft LANGUAGES C CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/utils.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_EDITOR_APP "enable editor app" OFF)
option(ENABLE_DESIGN_MODLE_CLI_TOOL "enable design model cli tool" OFF)

set(SEAT_CRAFT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SEAT_CRAFT_ROOT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(THIRD_PARTY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/third_party)

file(GLOB_RECURSE SEAT_CRAFT_COMMON_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/common/*.h)

# 强制 tgfx 走 Release
# set(CMAKE_BUILD_TYPE_RELEASE_SAVED ${CMAKE_BUILD_TYPE})
# set(CMAKE_BUILD_TYPE Release CACHE STRING "Force tgfx to Release" FORCE)

message(STATUS "ENABLE_EDITOR_APP: ${ENABLE_EDITOR_APP}")
message(STATUS "ENABLE_DESIGN_MODLE_CLI_TOOL: ${ENABLE_DESIGN_MODLE_CLI_TOOL}")

set(TGFX_BUILD_SVG ON)
set(TGFX_BUILD_LAYERS ON)

if(ENABLE_DESIGN_MODLE_CLI_TOOL)

elseif(ENABLE_EDITOR_APP)
    set(TGFX_USE_QT ON)
endif()

add_subdirectory(third_party/tgfx/third_party/json)
add_subdirectory(third_party/tgfx)

# 恢复原来的
# set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_RELEASE_SAVED} CACHE STRING "Restore build type" FORCE)

add_subdirectory(src/design_model)
add_subdirectory(src/editor_core)

if(ENABLE_DESIGN_MODLE_CLI_TOOL)
    add_subdirectory(tools/design_model_cli)
elseif(ENABLE_EDITOR_APP)
    add_subdirectory(src/editor_app)
else()
    add_subdirectory(src/app_picker)
endif()



