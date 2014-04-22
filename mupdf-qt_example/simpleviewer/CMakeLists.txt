project (simpleviewer)

# Qt5 library
set (CMAKE_AUTOMOC ON)
find_package (Qt5Widgets)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/libmupdf-qt)
link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

aux_source_directory (. project_SOURCES)

add_executable (${PROJECT_NAME} WIN32 ${project_SOURCES})
target_link_libraries (${PROJECT_NAME} mupdf-qt)
qt5_use_modules (${PROJECT_NAME} Widgets)
