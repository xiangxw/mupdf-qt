cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (test_document)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/libmupdf-qt)
link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

aux_source_directory (. project_SOURCES)

if (Qt5Widgets_FOUND)
	add_executable (${PROJECT_NAME} ${project_SOURCES})
	target_link_libraries (${PROJECT_NAME} mupdf-qt)
	qt5_use_modules (${PROJECT_NAME} Widgets)
else (Qt4_FOUND)
	add_executable (${PROJECT_NAME} ${project_SOURCES})
	target_link_libraries (${PROJECT_NAME} mupdf-qt)
	target_link_libraries (${PROJECT_NAME} ${QT_LIBRARIES})
endif ()
