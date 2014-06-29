project (test_document)

# Qt5 library
find_package (Qt5Gui)

# libmupdf-qt library
include_directories (${CMAKE_SOURCE_DIR}/include)
link_directories (${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} mupdf-qt)
qt5_use_modules (${PROJECT_NAME} Gui)
