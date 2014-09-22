cmake_minimum_required (VERSION 2.8.11 FATAL_ERROR)
project (mupdf-qt)

# Settings
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
cmake_policy (SET CMP0020 NEW) # link qtmain by default on win32 platform

# MuPDF library
find_package (MuPDF REQUIRED)
include_directories (${MuPDF_INCLUDE_DIRS})

# mupdf-qt library
set (LIBMUPDF mupdf-qt)
find_package (Qt5Gui)
include_directories (${CMAKE_SOURCE_DIR}/include)
include_directories (${CMAKE_SOURCE_DIR}/src/private)
set (LIBMUPDF_SOURCES src/mupdf-document.cpp
                      src/mupdf-page.cpp
                      src/mupdf-link.cpp
                      src/mupdf-outline.cpp
                      src/mupdf-textbox.cpp
                      )
add_library (${LIBMUPDF} STATIC ${LIBMUPDF_SOURCES})
target_link_libraries (${LIBMUPDF} ${MuPDF_LIBRARIES})
qt5_use_modules (${LIBMUPDF} Gui)

# Examples
add_subdirectory (examples)
