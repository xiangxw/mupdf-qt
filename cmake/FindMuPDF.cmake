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
endif ()

if (MuPDF_ROOT) # Using MuPDF root directory
	# Find include directory
	set (MuPDF_INCLUDE_DIRS_HINTS ${MuPDF_ROOT}/include /usr/local/mupdf/include /usr/local/include)
	find_path (MuPDF_INCLUDE_DIRS
		NAMES fitz.h fitz-internal.h memento.h
			mupdf.h mupdf-internal.h
			mucbz.h
			muxps.h muxps-internal.h
		PATHS ${MuPDF_INCLUDE_DIRS_HINTS})
	# Find library
	set (MuPDF_LIBRARY_HINTS ${MuPDF_ROOT}/lib /usr/local/mupdf/lib /usr/local/lib)
	if (UNIX)
		set (MuPDF_LIBRARY_COMPONENTS fitz jbig2dec jpeg openjpeg z)
	else ()
		set (MuPDF_LIBRARY_COMPONENTS fitz freetype jbig2dec jpeg openjpeg z)
	endif ()
	foreach (COMPONENT ${MuPDF_LIBRARY_COMPONENTS})
		find_library (${COMPONENT}_LIBRARY
			NAMES ${COMPONENT}
			PATHS ${MuPDF_LIBRARY_HINTS}
			NO_DEFAULT_PATH)
		if (NOT ${COMPONENT}_LIBRARY)
			message (FATAL_ERROR "Library " ${COMPONENT} " NOT FOUND!")
		endif ()
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${${COMPONENT}_LIBRARY})
	endforeach ()
	if (UNIX)
		# System provided freetype library should used in Linux.
		# If you use a different VERSION of freetype library with Qt used,
		# applications using libmupdf-qt may encounter some problems.
		find_library (freetype_LIBRARY freetype)
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${freetype_LIBRARY})
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} -lm)
	endif ()
elseif (MuPDF_SRC) # Using MuPDF source directory
	# Find include directory
	set (MuPDF_INCLUDE_DIRS ${MuPDF_SRC}/fitz ${MuPDF_SRC}/pdf ${MuPDF_SRC}/cbz ${MuPDF_SRC}/xps)
	# Find library
	if (UNIX)
		set (MuPDF_LIBRARY_COMPONENTS fitz jbig2dec jpeg openjpeg z)
	else ()
		set (MuPDF_LIBRARY_COMPONENTS fitz freetype jbig2dec jpeg openjpeg z)
	endif ()
	foreach (COMPONENT ${MuPDF_LIBRARY_COMPONENTS})
		find_library (${COMPONENT}_LIBRARY
			NAMES ${COMPONENT}
			PATHS ${MuPDF_SRC}/build/*
			NO_DEFAULT_PATH)
		if (NOT ${COMPONENT}_LIBRARY)
			message (FATAL_ERROR "Library " ${COMPONENT} " NOT FOUND!")
		endif ()
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${${COMPONENT}_LIBRARY})
	endforeach ()
	if (UNIX)
		# System provided freetype library should used in Linux.
		# If you use a different VERSION of freetype library with Qt used,
		# applications using libmupdf-qt may encounter some problems.
		find_library (freetype_LIBRARY freetype)
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} ${freetype_LIBRARY})
		set (MuPDF_LIBRARIES ${MuPDF_LIBRARIES} -lm)
	endif ()
endif ()

# Other
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (MuPDF MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
mark_as_advanced (MuPDF_INCLUDE_DIRS MuPDF_LIBRARIES)
