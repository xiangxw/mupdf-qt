cmake_minimum_required (VERSION 2.8)
project (countpages)
include_directories (/usr/local/mupdf/include) # modify when necessary
link_directories (/usr/local/mupdf/lib) # modify when necessary
add_executable (${PROJECT_NAME} main.c)
target_link_libraries (${PROJECT_NAME} mupdf fitz)
target_link_libraries (${PROJECT_NAME} freetype jbig2dec jpeg openjpeg z m)
