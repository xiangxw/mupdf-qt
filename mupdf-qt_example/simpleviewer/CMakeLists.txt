cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (simpleviewer)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/../../libmupdf-qt)
link_directories (${CMAKE_SOURCE_DIR}/../../libmupdf-qt/build)

# Qt Library
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

set (project_SOURCES main.cpp mainwindow.cpp)
set (project_HEADERS mainwindow.h)
QT4_WRAP_CPP (project_MOCS ${project_HEADERS})

add_executable (${PROJECT_NAME} main.cpp mainwindow.cpp ${project_MOCS})
target_link_libraries (${PROJECT_NAME} mupdf-qt)
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})
