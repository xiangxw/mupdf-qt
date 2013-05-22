cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
set (lib_NAME mupdf-qt)
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

aux_source_directory (. lib_SOURCES)

# Mupdf library
find_package (MuPDF REQUIRED)
include_directories (${MuPDF_INCLUDE_DIRS})

# Qt library
find_package (Qt5Gui) # Use Qt5 by default
if (NOT Qt5Gui_FOUND) # Use Qt4 when Qt5 is not found
	find_package (Qt4 REQUIRED)
	include (${QT_USE_FILE})
endif ()

add_library (${lib_NAME} SHARED ${lib_SOURCES})
target_link_libraries (${lib_NAME} ${MuPDF_LIBRARIES})
if (Qt5Gui_FOUND)
	qt5_use_modules (${lib_NAME} Gui)
else ()
	target_link_libraries (${lib_NAME} ${QT_LIBRARIES})
endif ()
