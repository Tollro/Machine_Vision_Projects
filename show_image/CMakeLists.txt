cmake_minimum_required(VERSION 3.19)
project(show_image LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)

qt_standard_project_setup()

qt_add_executable(show_image
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tracking_window.h tracking_window.cpp tracking_window.ui
)

target_link_libraries(show_image
    PRIVATE
        Qt::Core
        Qt::Widgets
        ${OpenCV_LIBS}
)

include_directories(${OpenCV_INCLUDE_DIRS})
include(GNUInstallDirs)

install(TARGETS show_image
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET show_image
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
