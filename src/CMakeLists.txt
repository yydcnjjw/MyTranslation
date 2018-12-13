set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5Network)
set(QT_LIB Qt5::Widgets Qt5::Network)

include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/thirdparty/qxtglobalshortcut5/include"
  ${})
add_executable(MyTranslation "")
target_sources(MyTranslation
  PRIVATE
  main.cc
  trayWidget.cc
  trayWidget.h
  translate.cc
  translate.h
  googleTranslate.cc
  googleTranslate.h
  http_util.cc
  http_util.h
  screen_shoot.cc
  screen_shoot.h)
target_link_libraries(MyTranslation qxtglobalshortcut5 ${QT_LIB})

