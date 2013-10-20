cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
set (lib_NAME mupdf-qt)

aux_source_directory (. lib_SOURCES)

add_library (${lib_NAME} STATIC ${lib_SOURCES})
target_link_libraries (${lib_NAME} ${MuPDF_LIBRARIES})
qt5_use_modules (${lib_NAME} Gui)
