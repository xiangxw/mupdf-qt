set (LIBMUPDF mupdf-qt)

# Qt5 library
find_package (Qt5Gui)

set (LIBMUPDF_SOURCES mupdf-document.cpp
	                  mupdf-page.cpp
					  mupdf-link.cpp
					  mupdf-outline.cpp)

add_library (${LIBMUPDF} STATIC ${LIBMUPDF_SOURCES})
target_link_libraries (${LIBMUPDF} ${MuPDF_LIBRARIES})
qt5_use_modules (${LIBMUPDF} Gui)
