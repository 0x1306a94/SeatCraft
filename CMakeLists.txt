cmake_minimum_required(VERSION 3.13)
project(SeatCraft LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SEAT_CRAFT_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SEAT_CRAFT_ROOT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(THIRD_PARTY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/third_party)

file(GLOB_RECURSE SEAT_CRAFT_COMMON_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/common/*.h)

# 强制 tgfx 走 Release
# set(CMAKE_BUILD_TYPE_RELEASE_SAVED ${CMAKE_BUILD_TYPE})
# set(CMAKE_BUILD_TYPE Release CACHE STRING "Force tgfx to Release" FORCE)

set(TGFX_BUILD_SVG ON)
set(TGFX_BUILD_LAYERS ON)
add_subdirectory(third_party/tgfx)

# 恢复原来的
# set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_RELEASE_SAVED} CACHE STRING "Restore build type" FORCE)

add_subdirectory(src/app_picker)
# add_subdirectory(src/editor)