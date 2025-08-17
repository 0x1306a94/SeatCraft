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

add_files_by_extension(APP_PICKER_CORE_FILES ".h;.hpp;.cpp"
    core
    core/thread
    core/renderer
    core/ui
    core/svg
    core/drawers
)

list(APPEND APP_PICKER_FILES ${APP_PICKER_CORE_FILES})

if(IOS)
    message(STATUS "IOS: ${IOS}")
    add_files_by_extension(APP_PICKER_PLATFORM_IOS_FILES ".h;.hpp;.m;.mm;.cpp"
        platform/ios
        platform/ios/thread
        platform/ios/renderer
    )
    list(APPEND APP_PICKER_FILES ${APP_PICKER_PLATFORM_IOS_FILES})
    
    file(GLOB_RECURSE APP_PICKER_PLATFORM_IOS_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR}/${PROJECT_NAME}/platform/ios/*.h)
    message(STATUS "APP_PICKER_PLATFORM_IOS_PUBLIC_HEADERS: ${APP_PICKER_PLATFORM_IOS_PUBLIC_HEADERS}")
    list(APPEND APP_PICKER_PUBLIC_HEADERS ${APP_PICKER_PLATFORM_IOS_PUBLIC_HEADERS})

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -exported_symbols_list ${SEAT_CRAFT_ROOT_DIR}/ios/SeatCraftAppPicker.lds")
elseif(ANDROID)
    message(STATUS "ANDROID: ${ANDROID}")
    find_library(LOG_LIB log)
    list(APPEND SEAT_CRAFT_APP_PICKER_SHARED_LIBS ${LOG_LIB})
    find_library(ANDROID_LIB android)
    list(APPEND SEAT_CRAFT_APP_PICKER_SHARED_LIBS ${ANDROID_LIB})

    add_files_by_extension(APP_PICKER_PLATFORM_ANDROID_FILES ".h;.hpp;.cpp"
        platform/android
        platform/android/jni
        platform/android/ui
        platform/android/thread
        platform/android/renderer
    )
    list(APPEND APP_PICKER_FILES ${APP_PICKER_PLATFORM_ANDROID_FILES})

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,max-page-size=16384 -Wl,--gc-sections -Wl,--version-script=${SEAT_CRAFT_ROOT_DIR}/android/SeatCraftAppPicker.def")
elseif(DEFINED EMSCRIPTEN)
    list(APPEND APP_PICKER_PLATFORM_WEB_COMPILE_OPTIONS -fno-rtti -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0)
    list(APPEND APP_PICKER_PLATFORM_WEB_LINK_OPTIONS --no-entry -lembind -fno-rtti -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0 -sEXPORT_NAME='${PROJECT_NAME}' -sWASM=1
            -sMAX_WEBGL_VERSION=2 -sEXPORTED_RUNTIME_METHODS=['GL','HEAPU8'] -sMODULARIZE=1
            -sENVIRONMENT=web,worker -sEXPORT_ES6=1)
    set(unsupported_emsdk_versions "4.0.11")
    foreach (unsupported_version IN LISTS unsupported_emsdk_versions)
        if (${EMSCRIPTEN_VERSION} VERSION_EQUAL ${unsupported_version})
            message(FATAL_ERROR "Emscripten version ${EMSCRIPTEN_VERSION} is not supported.")
        endif ()
    endforeach ()
    if(EMSCRIPTEN_PTHREADS)
        list(APPEND APP_PICKER_PLATFORM_WEB_LINK_OPTIONS -sUSE_PTHREADS=1 -sINITIAL_MEMORY=32MB -sALLOW_MEMORY_GROWTH=1
                -sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency -sPROXY_TO_PTHREAD=1
                -sEXIT_RUNTIME=0 -sINVOKE_RUN=0 -sMALLOC=mimalloc)
        list(APPEND APP_PICKER_PLATFORM_WEB_COMPILE_OPTIONS -fPIC -pthread)
    else()
        list(APPEND APP_PICKER_PLATFORM_WEB_LINK_OPTIONS -sALLOW_MEMORY_GROWTH=1)
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND APP_PICKER_PLATFORM_WEB_COMPILE_OPTIONS -O0 -g3)
        list(APPEND APP_PICKER_PLATFORM_WEB_LINK_OPTIONS -O0 -g3 -sSAFE_HEAP=1 -Wno-limited-postlink-optimizations)
    else ()
        list(APPEND APP_PICKER_PLATFORM_WEB_COMPILE_OPTIONS -Oz)
        list(APPEND APP_PICKER_PLATFORM_WEB_LINK_OPTIONS -Oz)
    endif ()

     add_files_by_extension(APP_PICKER_PLATFORM_WEB_FILES ".h;.hpp;.cpp"
        platform/web
        platform/web/thread
        platform/web/renderer
    )
    list(APPEND APP_PICKER_FILES ${APP_PICKER_PLATFORM_WEB_FILES})
endif()

add_source_group(SEAT_CRAFT_COMMON_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR} "PublicHeaders")
add_source_group(APP_PICKER_PUBLIC_HEADERS ${SEAT_CRAFT_ROOT_INCLUDE_DIR} "PublicHeaders")
add_source_group(APP_PICKER_FILES ${CMAKE_CURRENT_SOURCE_DIR} "Sources")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")

if(DEFINED EMSCRIPTEN)
    add_executable(${PROJECT_NAME} ${SEAT_CRAFT_COMMON_PUBLIC_HEADERS} ${APP_PICKER_PUBLIC_HEADERS} ${APP_PICKER_FILES})
else()
    add_library(${PROJECT_NAME} SHARED ${SEAT_CRAFT_COMMON_PUBLIC_HEADERS} ${APP_PICKER_PUBLIC_HEADERS} ${APP_PICKER_FILES})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE ${SEAT_CRAFT_APP_PICKER_COMPILE_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SEAT_CRAFT_ROOT_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_ROOT_DIR}/tgfx/include)
target_include_directories(${PROJECT_NAME} PRIVATE .)
target_link_libraries(${PROJECT_NAME} PRIVATE tgfx)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SEAT_CRAFT_APP_PICKER_SHARED_LIBS})
target_compile_options(${PROJECT_NAME} PUBLIC ${APP_PICKER_PLATFORM_WEB_COMPILE_OPTIONS})
target_link_options(${PROJECT_NAME} PUBLIC ${APP_PICKER_PLATFORM_WEB_LINK_OPTIONS})

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
