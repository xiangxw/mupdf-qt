cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (qimage)

add_executable (${PROJECT_NAME} main.cpp)
target_link_libraries (${PROJECT_NAME} ${MuPDF_LIBRARIES})
qt5_use_modules (${PROJECT_NAME} Widgets)
