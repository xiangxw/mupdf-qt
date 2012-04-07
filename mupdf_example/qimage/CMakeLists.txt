cmake_minimum_required (VERSION 2.8)
project (qimage)

include_directories (${CMAKE_SOURCE_DIR}/../../libmupdf/include)
link_directories (${CMAKE_SOURCE_DIR}/../../libmupdf/lib)

find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} fitz)
target_link_libraries (${PROJECT_NAME} freetype jbig2dec jpeg openjpeg z m)
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})
