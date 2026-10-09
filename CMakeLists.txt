cmake_minimum_required(VERSION 3.16)
project(hextool VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Gui Widgets)

qt_standard_project_setup()

qt_add_executable(hextool WIN32 MACOSX_BUNDLE
    common/debug.h
    common/handlebinarydigit.c common/handlebinarydigit.h
    common/numbits.c common/numbits.h
    common/register.c common/register.h
    common/registeroperation.c common/registeroperation.h
    common/signed.c common/signed.h
    common/value.c common/value.h
    copydialog.cpp copydialog.h copydialog.ui
    hextool.cpp hextool.h hextool.ui
    main.cpp
    optionsdialog.cpp optionsdialog.h optionsdialog.ui
)
target_include_directories(hextool PRIVATE
    /usr/include/qt6
)

target_compile_definitions(hextool PRIVATE
    Q_WS_MAEMO_5
)

target_link_libraries(hextool PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    qt6maemo
)


# Resources:
set(image_resource_files
    "deploy/icons/48x48/hextool.png"
)

qt_add_resources(hextool "image"
    PREFIX
        "/new/ownresource"
    FILES
        ${image_resource_files}
)

install(FILES deploy/leste/hextool.desktop
        DESTINATION /usr/share/applications/hildon
)

install(FILES deploy/icons/64x64/hextool.png
        DESTINATION /usr/share/icons/hicolor/64x64/apps
)

install(PROGRAMS deploy/leste/hextool.sh
        DESTINATION /usr/share/hextool
)

install(TARGETS hextool
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
