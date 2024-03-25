cmake_minimum_required(VERSION 3.27)
project(DAC)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

list(APPEND CMAKE_AUTOMOC_SEARCH_PATHS "src/Qt/src")
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "src/Qt/ui")

message("Github Action: " ${GITHUB_ACTION})

if (GITHUB_ACTION)
    set(CMAKE_PREFIX_PATH "D:/a/maze-algorithm-visualization/Qt/6.5.2/mingw_64")
else ()
    set(CMAKE_PREFIX_PATH "D:/Qt/6.5.2/mingw_64")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif ()

if (NO_CONSOLE)
    add_compile_definitions(NO_CONSOLE)
endif ()

message("Build Type: " ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS})

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/include
        ${PROJECT_SOURCE_DIR}/src/Qt/include
        ${PROJECT_SOURCE_DIR}/src/BmpFile/include
        ${PROJECT_SOURCE_DIR}/src/Backend/include
        ${PROJECT_SOURCE_DIR}/src/Algorithm/include
        ${PROJECT_SOURCE_DIR}/src/Factory/include
        ${PROJECT_SOURCE_DIR}/src/Thread/include
)

find_package(Qt6 COMPONENTS
        Core REQUIRED
        Gui REQUIRED
        Widgets REQUIRED)

file(GLOB_RECURSE HEADERS
        include/*
        src/include/*
        src/Qt/include/*
        src/Backend/include/*
        src/BmpFile/include/*
        src/Algorithm/include/*
        src/Factory/include/*
        src/Thread/include/*
)

file(GLOB_RECURSE SOURCE
        src/Qt/src/*
        src/Backend/src/*
        src/BmpFile/src/*
        src/Algorithm/src/*
        src/Factory/src/*
        src/Thread/src/*
)

file(GLOB_RECURSE QRC_FILES resource/*.qrc)
file(GLOB_RECURSE FORMS src/Qt/ui/*)
file(GLOB_RECURSE STATIC_LIBS lib/static/*)
file(GLOB_RECURSE SHARED_LIBS lib/shared/*)

if (NO_CONSOLE)
    add_executable(${PROJECT_NAME} WIN32 icon.rc
            main.cpp
            ${QRC_FILES}
            ${HEADERS}
            ${SOURCE}
            ${FORMS}
    )
else ()
    add_executable(${PROJECT_NAME} icon.rc
            main.cpp
            ${QRC_FILES}
            ${HEADERS}
            ${SOURCE}
            ${FORMS}
    )
endif ()

link_libraries(${PROJECT_NAME} ${SHARED_LIBS})

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${STATIC_LIBS}
)

foreach (LIB_DIR ${PROJECT_SOURCE_DIR}/lib/static ${PROJECT_SOURCE_DIR}/lib/include ${PROJECT_SOURCE_DIR}/lib/shared)
    add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${LIB_DIR})
endforeach (LIB_DIR)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/lib/static"
        "${PROJECT_SOURCE_DIR}/bin")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/lib/include"
        "${PROJECT_SOURCE_DIR}/bin")


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
