cmake_minimum_required(VERSION 3.13)
project(SeatCraftAppPicker) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0063 NEW)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(SEAT_CRAFT_APP_PICKER_VERSION "1.0.0")

if(MACOS OR IOS)
  enable_language(OBJC OBJCXX)
  option(SEAT_CRAFT_APP_PICKER_BUILD_FRAMEWORK "Enable building framework" ON)
else()
  set(SEAT_CRAFT_APP_PICKER_BUILD_FRAMEWORK OFF)
endif()


file(GLOB_RECURSE APP_PICKER_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/platform/ios/*.h)
message(STATUS "APP_PICKER_PUBLIC_HEADERS: ${APP_PICKER_PUBLIC_HEADERS}")

file(GLOB_RECURSE APP_PICKER_PRIVATE_HEADERS ./*.h)
list(APPEND APP_PICKER_FILES ${APP_PICKER_PRIVATE_HEADERS})

file(GLOB_RECURSE APP_PICKER_CORE_FILES
    core/*.h core/*.hpp core/*.cpp
    core/ui/*.h core/ui/*.hpp core/ui/*.cpp
    core/svg/*.h core/svg/*.hpp core/svg/*.cpp
    core/renderer/*.h core/renderer/*.hpp core/renderer/*.cpp
    core/drawers/*.h core/drawers/*.hpp core/drawers/*.cpp
)

list(APPEND APP_PICKER_FILES ${APP_PICKER_CORE_FILES})

if(IOS)
    file(GLOB_RECURSE APP_PICKER_PLATFORM_IOS_FILES
        platform/ios/*.h platform/ios/*.m platform/ios/*.mm
        platform/ios/renderer/*.h platform/ios/renderer/*.m platform/ios/renderer/*.mm
    )
    list(APPEND APP_PICKER_FILES ${APP_PICKER_PLATFORM_IOS_FILES})

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -exported_symbols_list ${SEAT_CRAFT_ROOT_DIR}/ios/SeatCraftAppPicker.lds")
endif()

foreach(source_file IN LISTS APP_PICKER_FILES)
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${source_file}")
    get_filename_component(group_path "${rel_path}" DIRECTORY)
    if(group_path STREQUAL "")
        set(group_path "Root")
    endif()
    source_group("${group_path}" FILES "${source_file}")
endforeach()

foreach(header IN LISTS SEAT_CRAFT_COMMON_PUBLIC_HEADERS)
    file(RELATIVE_PATH rel_path "${SEAT_CRAFT_ROOT_INCLUDE_DIR}" "${header}")
    get_filename_component(group_path "${rel_path}" DIRECTORY)
    if(group_path STREQUAL "")
        set(group_path "PublicHeaders")
    endif()
    source_group("PublicHeaders\\${group_path}" FILES "${header}")
endforeach()

foreach(header IN LISTS APP_PICKER_PUBLIC_HEADERS)
    file(RELATIVE_PATH rel_path "${SEAT_CRAFT_ROOT_INCLUDE_DIR}" "${header}")
    get_filename_component(group_path "${rel_path}" DIRECTORY)
    if(group_path STREQUAL "")
        set(group_path "PublicHeaders")
    endif()
    source_group("PublicHeaders\\${group_path}" FILES "${header}")
endforeach()

foreach(source_file IN LISTS APP_PICKER_FILES)
    file(RELATIVE_PATH rel_path "${CMAKE_CURRENT_SOURCE_DIR}" "${source_file}")
    get_filename_component(group_path "${rel_path}" DIRECTORY)
    if(group_path STREQUAL "")
        set(group_path "Source")
    endif()
    source_group("Source\\${group_path}" FILES "${source_file}")
endforeach()

add_library(${PROJECT_NAME} SHARED ${SEAT_CRAFT_COMMON_PUBLIC_HEADERS} ${APP_PICKER_PUBLIC_HEADERS} ${APP_PICKER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SEAT_CRAFT_ROOT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_ROOT_DIR}/tgfx/include)
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE tgfx)

if(MACOS OR IOS)
    set_target_properties(${PROJECT_NAME} PROPERTIES
      XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES"
      XCODE_ATTRIBUTE_MODULE_VERIFIER_SUPPORTED_LANGUAGES "objective-c objective-c++"
      XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING "YES"
      XCODE_ATTRIBUTE_DEPLOYMENT_POSTPROCESSING[config=Debug] "NO"
      XCODE_ATTRIBUTE_COPY_PHASE_STRIP "YES"
      XCODE_ATTRIBUTE_STRIP_INSTALLED_PRODUCT "YES"
      XCODE_ATTRIBUTE_STRIP_STYLE "non-global"
    )
endif()

if (SEAT_CRAFT_APP_PICKER_BUILD_FRAMEWORK)
    set(SEAT_CRAFT_APP_PICKER_IDENTIFIER "com.seatcraft.apppicker")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        FRAMEWORK TRUE
        INSTALL_NAME_DIR "@rpath/${PROJECT_NAME}.framework/${PROJECT_NAME}"
        INSTALL_RPATH "@executable_path/Frameworks;@loader_path/Frameworks"
        XCODE_ATTRIBUTE_FRAMEWORK_VERSION A
        XCODE_ATTRIBUTE_WRAPPER_EXTENSION "framework"
        XCODE_ATTRIBUTE_DEFINES_MODULE "YES"
        XCODE_ATTRIBUTE_CLANG_ENABLE_MODULES "YES"
        XCODE_ATTRIBUTE_MODULEMAP_FILE "${CMAKE_CURRENT_LIST_DIR}/module.modulemap"
        XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
        XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER ${SEAT_CRAFT_APP_PICKER_IDENTIFIER}
        MACOSX_FRAMEWORK_IDENTIFIER ${SEAT_CRAFT_APP_PICKER_IDENTIFIER}
        MACOSX_FRAMEWORK_BUNDLE_VERSION ${SEAT_CRAFT_APP_PICKER_VERSION}
        MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${SEAT_CRAFT_APP_PICKER_VERSION}
        MACOSX_FRAMEWORK_BUNDLE_NAME ${PROJECT_NAME}
    )

    foreach (header ${APP_PICKER_PUBLIC_HEADERS})
        file(RELATIVE_PATH dir ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/platform/ios/ ${header})
        get_filename_component(dir "${dir}" DIRECTORY)
        if (dir)
            set(location "Headers/${dir}")
        else ()
            set(location "Headers")
        endif ()
        set_source_files_properties(${header} PROPERTIES MACOSX_PACKAGE_LOCATION "${location}")
    endforeach ()
endif ()