# You can use following variables
#	MuPDF_FOUND
#	MuPDF_INCLUDE_DIRS
#	MuPDF_LIBRARIES


# Get MuPDF root directory
if (NOT $ENV{MuPDF_ROOT} STREQUAL "")
	set (MuPDF_ROOT $ENV{MuPDF_ROOT})
endif ()

# Get MuPDF source directory
if (NOT $ENV{MuPDF_SRC} STREQUAL "")
	set (MuPDF_SRC $ENV{MuPDF_SRC})
endif()

if (MuPDF_ROOT) # Using MuPDF root directory
	# Find include directory
	set (MuPDF_INCLUDE_DIRS_HINTS ${MuPDF_ROOT}/include /usr/local/mupdf/include /usr/local/include /usr/include)
	find_path (MuPDF_INCLUDE_DIRS
		NAMES fitz.h fitz-internal.h memento.h
			mupdf.h mupdf-internal.h
			mucbz.h
			muxps.h muxps-internal.h
		PATHS ${MuPDF_INCLUDE_DIRS_HINTS})
	# Find library
	set (MuPDF_LIBRARY_HINTS ${MuPDF_ROOT}/lib /usr/local/mupdf/lib /usr/local/lib /usr/lib)
	set (MuPDF_LIBRARY_COMPONENTS fitz freetype jbig2dec jpeg openjpeg z)
	foreach (COMPONENT ${MuPDF_LIBRARY_COMPONENTS})
		find_library (${COMPONENT}_LIBRARY
			NAMES ${COMPONENT}
			PATHS ${MuPDF_LIBRARY_HINTS})
		if (NOT ${COMPONENT}_LIBRARY)
			message (FATAL_ERROR "Library " ${COMPONENT} " NOT FOUND!")
		endif()
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${${COMPONENT}_LIBRARY})
	endforeach()
elseif (MUPDF_SRC) # Using MuPDF source directory
	# Find include directory
	set (MuPDF_INCLUDE_DIRS ${MuPDF_SRC}/fitz ${MuPDF_SRC}/pdf ${MuPDF_SRC}/cbz ${MuPDF_SRC}/xps)
	# Find library
	set (MuPDF_LIBRARIES ${MuPDF_SRC}/build/debug/libfitz.a
		${MuPDF_SRC}/build/debug/libfreetype.a
		${MuPDF_SRC}/build/debug/libjbig2dec.a
		${MuPDF_SRC}/build/debug/libjpeg.a
		${MuPDF_SRC}/build/debug/libopenjpeg.a
		${MuPDF_SRC}/build/debug/libz.a)
endif()

# Other
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (MuPDF MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
mark_as_advanced (MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
