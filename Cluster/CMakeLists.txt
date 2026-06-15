cmake_minimum_required(VERSION 3.16)
project(Cluster VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)          # safer than 17 on QCC
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/qml" CACHE STRING "Extra QML Import Paths" FORCE)

find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.9)

qt_add_executable(appCluster
    main.cpp
)

qt_add_qml_module(appCluster
    URI Cluster

    QML_FILES
        Main.qml

    SOURCES
        cluster.h
        cluster.cpp

    RESOURCES
        images.qrc

)

set_target_properties(appCluster PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCluster
    PRIVATE Qt6::Quick
)

# ── QNX-specific ──────────────────────────────────────────────────────────
if(QNX)
    target_compile_definitions(appCluster PRIVATE __QNXNTO__)

    target_compile_options(appCluster PRIVATE
        -V${QNX_TARGET_ARCH}    # e.g. aarch64le or x86_64
        -lang-c++
        -Wc,-std=c++14
    )

    target_link_libraries(appCluster PRIVATE
        slog2          # QNX system logger (slog2f calls in cluster.cpp)
        socket         # needed by Qt networking on QNX
    )

    # QNX deploy path — adjust to match your image layout
    install(TARGETS appCluster
        RUNTIME DESTINATION /apps/cluster
    )
else()
    install(TARGETS appCluster
        BUNDLE  DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
# Fix slog2 linking
if(QNX)
target_link_libraries(appCluster PRIVATE
    /home/yasmine/qnx-qemu/qnx800/target/qnx/x86_64/lib/libslog2.so.1
)
endif()
