project (qimage)

# Qt5 library
find_package (Qt5Widgets)

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} ${MuPDF_LIBRARIES})
qt5_use_modules (${PROJECT_NAME} Widgets)
