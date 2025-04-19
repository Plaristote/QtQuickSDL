set(test_libraries
  ${TARGET_NAME}
  Qt${QT_MAJOR_VERSION}::Core
  Qt${QT_MAJOR_VERSION}::Quick
  ${SDL2_LIBRARIES}
  SDL2_image::SDL2_image
)

include_directories(../src)

qt_add_executable(tilemapTest
  tst_tilemap.cpp
  tst_tilemap.h
  tst.qrc)
qt_add_qml_module(tilemapTest
  URI QtQuickSDLTestTilemap
  VERSION 1.0
  QML_FILES Main.qml)

target_link_libraries(tilemapTest PRIVATE ${test_libraries})

qt_add_executable(triangleTest
  tst_triangle.cpp)
qt_add_qml_module(triangleTest
  URI QtQuickSDLTestTriangle
  VERSION 1.0
  QML_FILES Main.qml)

target_link_libraries(triangleTest PRIVATE ${test_libraries})
