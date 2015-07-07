project (road)
cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

#QT LIBS

find_package(Qt4 COMPONENTS QTCORE QTGUI QTOPENGL)

find_package(OpenGL)
#find_package(QtOpenGL)

add_executable(road main.cpp myglwidget.cpp mainwindow.cpp)

target_link_libraries(road ${QT_LIBRARIES} ${OPENGL_LIBRARIES})