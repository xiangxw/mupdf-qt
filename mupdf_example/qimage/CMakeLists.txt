cmake_minimum_required (VERSION 2.8)
project (qimage)

include_directories (/usr/local/mupdf/include/) #modify if necessary
link_directories (/usr/local/mupdf/lib/) #modify if necessary
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

set (project_SOURCES main.cpp)
add_executable (${PROJECT_NAME} ${project_SOURCES})

target_link_libraries (${PROJECT_NAME} mupdf fitz)
target_link_libraries (${PROJECT_NAME} freetype jbig2dec jpeg openjpeg z m)
target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})
