cmake_minimum_required(VERSION 3.11)
project(qxtglobalshortcut5)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
if(UNIX)
  find_package(Qt5X11Extras)
endif()

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
  ${Qt5Gui_PRIVATE_INCLUDE_DIRS})

add_library(qxtglobalshortcut5 SHARED ${QXTGLOBALSHORTCUT5_SRC})
target_link_libraries(qxtglobalshortcut5 PRIVATE Qt5::Widgets Qt5::X11Extras)
target_sources(qxtglobalshortcut5 PRIVATE
  include/qxtglobalshortcut.h
  include/qxtglobal.h
  gui/qxtglobalshortcut_p.h
  gui/qxtwindowsystem.h
  core/qxtglobal.cpp
  gui/qxtwindowsystem.cpp
  gui/qxtglobalshortcut.cpp)

if(UNIX)
  target_sources(qxtglobalshortcut5 PRIVATE
    gui/qxtglobalshortcut_x11.cpp
    gui/qxtwindowsystem_x11.cpp
    gui/x11info.h)
elseif(APPLE)
  target_sources(qxtglobalshortcut5 PRIVATE
    gui/qxtwindowsystem_mac.cpp
    gui/qxtglobalshortcut_mac.cpp
    qxtwindowsystem_mac.h)
elseif(MSVC)
  target_sources(qxtglobalshortcut5 PRIVATE
    gui/qxtwindowsystem_win.cpp
    gui/qxtglobalshrotcut_win.cpp)
endif()

