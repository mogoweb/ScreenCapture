cmake_minimum_required(VERSION 3.10)

project(ScreenCapture VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SKIA_SDK_PATH "D:\\sdk\\skia" CACHE PATH "Skia include path,default:D:/sdk/skia" FORCE)

set(MY_SYSTEM_FOLDER "win/x86")
if(CMAKE_ARCH_ABI STREQUAL "x86_64")
    set(MY_SYSTEM_FOLDER "win/x64")
endif()

set(SKIA_INCLUDE_PATH "${SKIA_SDK_PATH}")
set(SKIA_LIB_PATH "${SKIA_SDK_PATH}/${MY_SYSTEM_FOLDER}/${CMAKE_BUILD_TYPE}/lib")

message(STATUS "skia lib path: ${SKIA_LIB_PATH}")

if(NOT IS_DIRECTORY "${SKIA_INCLUDE_PATH}")
    message(FATAL_ERROR "The path ${SKIA_INCLUDE_PATH} does not exist.")
endif()

if(NOT IS_DIRECTORY "${SKIA_LIB_PATH}")
    message(FATAL_ERROR "The path ${SKIA_LIB_PATH} does not exist.")
endif()

link_directories("${SKIA_LIB_PATH}")
include_directories("${SKIA_INCLUDE_PATH}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (MY_RESOURCES res/res.rc)
endif()

set(PROJECT_SOURCES
        src/App.cpp
        src/App.h
        src/ColorBlender.cpp
        src/ColorBlender.h
        src/Cursor.h
        src/CutMask.cpp
        src/CutMask.h
        src/Icon.h
        src/main.cpp
        src/PixelInfo.cpp
        src/PixelInfo.h
        src/Recorder.cpp
        src/Recorder.h
        src/State.h
        src/ToolBase.cpp
        src/ToolBase.h
        src/ToolBtn.cpp
        src/ToolBtn.h
        src/ToolMain.cpp
        src/ToolMain.h
        src/ToolSub.cpp
        src/ToolSub.h
        src/WindowBase.cpp
        src/WindowBase.h
        src/WindowMain.cpp
        src/WindowMain.h
        src/WindowPin.cpp
        src/WindowPin.h
        src/Shape/ShapeArrow.cpp
        src/Shape/ShapeArrow.h
        src/Shape/ShapeBase.cpp
        src/Shape/ShapeBase.h
        src/Shape/ShapeEllipse.cpp
        src/Shape/ShapeEllipse.h
        src/Shape/ShapeEraser.cpp
        src/Shape/ShapeEraser.h
        src/Shape/ShapeEraserRect.cpp
        src/Shape/ShapeEraserRect.h
        src/Shape/ShapeLine.cpp
        src/Shape/ShapeLine.h
        src/Shape/ShapeMosaic.cpp
        src/Shape/ShapeMosaic.h
        src/Shape/ShapeMosaicRect.cpp
        src/Shape/ShapeMosaicRect.h
        src/Shape/ShapeNumber.cpp
        src/Shape/ShapeNumber.h
        src/Shape/ShapePen.cpp
        src/Shape/ShapePen.h
        src/Shape/ShapeRect.cpp
        src/Shape/ShapeRect.h
        src/Shape/ShapeText.cpp
        src/Shape/ShapeText.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ScreenCapture
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ScreenCapture APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(ScreenCapture SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ScreenCapture
            ${PROJECT_SOURCES}
            ${MY_RESOURCES}
        )
    endif()
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

target_link_libraries(ScreenCapture PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
    set_property(TARGET ScreenCapture PROPERTY VS_DPI_AWARE "PerMonitor")
    target_compile_definitions(ScreenCapture PRIVATE $<$<CONFIG:Debug>:UNICODE _UNICODE DEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_DEBUG WIN32 _WINDOWS>)
    target_compile_definitions(ScreenCapture PRIVATE $<$<CONFIG:Release>:UNICODE _UNICODE NDEBUG NOMINMAX _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS SK_RELEASE WIN32 _WINDOWS>)
    target_link_libraries(ScreenCapture PRIVATE skia Dwmapi winmm kernel32 user32 shell32 imm32 gdi32 Usp10)
elseif(APPLE)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ScreenCapture)
endif()
set_target_properties(ScreenCapture PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ScreenCapture
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ScreenCapture)
endif()
