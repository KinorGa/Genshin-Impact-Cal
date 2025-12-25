cmake_minimum_required(VERSION 3.21...3.30)

project(GiCal VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick QuickControls2 Graphs Charts)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(third_party/yaml-cpp)
# target_include_directories(yaml-cpp PUBLIC third_party/yaml-cpp/src)
add_compile_options(/utf-8)
qt_add_executable(appGiCal
    main.cpp
    gical.qrc
)

qt_add_qml_module(appGiCal
    URI GiCal
    VERSION 1.0
    QML_FILES
        ui/Main.qml
        ui/RelicComboBox.qml
        ui/RelicStatModel.qml
        ui/SingleRelicPanel.qml
        ui/RelicPanel.qml
        ui/BufferBox.qml
        ui/DynamicBox.qml
        ui/Buffer.qml
        ui/BufferPage.qml
        ui/MenuRec.qml
    SOURCES
        src/states.hpp
        src/states.cpp  
)

set_target_properties(appGiCal PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGiCal
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::Graphs Qt6::Charts yaml-cpp
)

include(GNUInstallDirs)
install(TARGETS appGiCal
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
