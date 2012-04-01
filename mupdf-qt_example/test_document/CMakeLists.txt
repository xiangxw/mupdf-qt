cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (test_document)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/../../libmupdf-qt)
link_directories (${CMAKE_SOURCE_DIR}/../../libmupdf-qt/build)

# Qt Library
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} mupdf-qt)
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})
