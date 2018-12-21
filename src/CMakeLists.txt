set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5Network)
set(QT_LIB Qt5::Widgets Qt5::Network)

include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/thirdparty/qxtglobalshortcut5/include")

add_executable(MyTranslation "")
target_sources(MyTranslation
  PRIVATE
  main.cc
  tray_widget.cc
  tray_widget.h
  translation_widget.cc
  translation_widget.h
  shortcut_manager.cc
  shortcut_manager.h
  status.h
  status.cc
  translate.cc
  translate.h
  google_translate.cc
  google_translate.h
  http_util.cc
  http_util.h
  no_destructor.h
  screen_shoot.cc
  screen_shoot.h)
target_link_libraries(MyTranslation qxtglobalshortcut5 ${QT_LIB})

