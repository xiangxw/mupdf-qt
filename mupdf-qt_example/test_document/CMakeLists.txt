project (test_document)

# Qt5 library
find_package (Qt5Gui)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/libmupdf-qt)
link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

aux_source_directory (. project_SOURCES)

add_executable (${PROJECT_NAME} ${project_SOURCES})
target_link_libraries (${PROJECT_NAME} mupdf-qt)
qt5_use_modules (${PROJECT_NAME} Gui)
