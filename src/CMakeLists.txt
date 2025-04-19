cmake_minimum_required(VERSION 3.16)

file(GLOB cpp_sources *.cpp)
file(GLOB cpp_headers *.h)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_qml_module(${TARGET_NAME}
  URI QtQuickSDL
  VERSION 1.0
  SOURCES ${cpp_sources} ${cpp_headers}
)

target_compile_definitions(QtQuickSDL PRIVATE QSDL_LIBRARY)
if (${SDL2_image_FOUND})
  target_compile_definitions(QtQuickSDL PRIVATE WITH_SDL_IMAGE)
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(QtQuickSDL PROPERTIES
#  MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQtQuickSDL
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)

target_link_libraries(${TARGET_NAME}
  PRIVATE
  Qt${QT_MAJOR_VERSION}::Core
  Qt${QT_MAJOR_VERSION}::Quick
  ${SDL2_LIBRARIES}
  SDL2_image::SDL2_image
)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${cpp_headers}
  DESTINATION include/QtQuickSDL)
