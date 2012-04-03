cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
set (lib_NAME mupdf-qt)
set (lib_SOURCES
	mupdf-global.h
	mupdf-document.h
	mupdf-document_p.h
	mupdf-document.cpp
	mupdf-page.h
	mupdf-page_p.h
	mupdf-page.cpp
	)

# Mupdf library
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../libmupdf/include) #modify if necessary
link_directories (${CMAKE_CURRENT_SOURCE_DIR}/../libmupdf/lib) #modify if necessary

# Qt4 library
find_package (Qt4 REQUIRED)
include (${QT_USE_FILE})

add_library (${lib_NAME} SHARED ${lib_SOURCES})
target_link_libraries (${lib_NAME} fitz)
target_link_libraries (${lib_NAME} freetype jbig2dec jpeg openjpeg z m)
target_link_libraries (${lib_NAME} ${QT_LIBRARIES})
