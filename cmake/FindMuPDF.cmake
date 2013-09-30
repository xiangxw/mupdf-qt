# You can use following variables
#	MuPDF_FOUND
#	MuPDF_INCLUDE_DIRS
#	MuPDF_LIBRARIES

# Find MuPDF source directory
set (MuPDF_SOURCE "mupdf-1.3-source")
set (MuPDF_SOURCE_DIR ${CMAKE_SOURCE_DIR}/${MuPDF_SOURCE})
if (NOT EXISTS ${MuPDF_SOURCE_DIR})
	message (FATAL_ERROR ${MuPDF_SOURCE_DIR} " not found! ")
endif ()

# Find include directory
set (MuPDF_INCLUDE_DIRS ${MuPDF_SOURCE_DIR}/include)

# Find libraries
if (MSVC)
	set (MuPDF_LIBRARY_COMPONENTS mupdf mupdf-js-none thirdparty)
	set (MuPDF_LIBRARY_PATHS ${MuPDF_SOURCE_DIR}/platform/win32/*)
else ()
	set (MuPDF_LIBRARY_COMPONENTS mupdf mupdf-js-none freetype jbig2dec jpeg openjpeg z)
	set (MuPDF_LIBRARY_PATHS ${MuPDF_SOURCE_DIR}/build/*)
endif ()
foreach (MuPDF_LIBRARY_COMPONENT ${MuPDF_LIBRARY_COMPONENTS})
	find_library (${MuPDF_LIBRARY_COMPONENT}_LIB
		${MuPDF_LIBRARY_COMPONENT}
		PATHS ${MuPDF_LIBRARY_PATHS}
		NO_DEFAULT_PATH)
	if (NOT ${MuPDF_LIBRARY_COMPONENT}_LIB)
		message (FATAL_ERROR "Library " ${MuPDF_LIBRARY_COMPONENT} " not found in " ${MuPDF_LIBRARY_PATH})
	endif ()
	set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${${MuPDF_LIBRARY_COMPONENT}_LIB})
endforeach () 
if (UNIX)
	set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} -lm)
endif ()

# Other
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (MuPDF MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
mark_as_advanced (MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
