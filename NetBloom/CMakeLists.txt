cmake_minimum_required(VERSION 3.16)

project(NetBloom VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick DBus)
find_package(PkgConfig REQUIRED)
pkg_check_modules(NM REQUIRED libnm)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appNetBloom
    main.cpp
    WifiManager.h
    WifiManager.cpp
    BluetoothManager.h
    BluetoothManager.cpp
)

qt_add_qml_module(appNetBloom
    URI NetBloom
    QML_FILES
        Main.qml
        WifiPage.qml
        BluetoothPage.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appNetBloom PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appNetBloom
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appNetBloom
    PRIVATE Qt6::Quick
    Qt6::DBus
    ${NM_LIBRARIES}
)
target_include_directories(appNetBloom PRIVATE
    ${NM_INCLUDE_DIRS}
)

target_compile_options(appNetBloom PRIVATE
    ${NM_CFLAGS_OTHER}
)

include(GNUInstallDirs)
install(TARGETS appNetBloom
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
