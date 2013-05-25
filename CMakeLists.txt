cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (mupdf-qt)

# Settings
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Mupdf library
find_package (MuPDF REQUIRED)
include_directories (${MuPDF_INCLUDE_DIRS})

# Qt library
find_package (Qt5Widgets) # Use Qt5 by default
if (Qt5Widgets_FOUND)
	set (CMAKE_AUTOMOC ON)
else () # Use Qt4 when Qt5 is not found
	find_package (Qt4 REQUIRED)
	include (${QT_USE_FILE})
endif ()

# Add directories
add_subdirectory (libmupdf-qt)
add_subdirectory (mupdf_example)
add_subdirectory (mupdf-qt_example)
