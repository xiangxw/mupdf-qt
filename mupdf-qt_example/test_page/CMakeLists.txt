cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (test_page)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/libmupdf-qt)
link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

aux_source_directory (. project_SOURCES)

add_executable (${PROJECT_NAME} ${project_SOURCES})
target_link_libraries (${PROJECT_NAME} mupdf-qt)
qt5_use_modules (${PROJECT_NAME} Widgets)
