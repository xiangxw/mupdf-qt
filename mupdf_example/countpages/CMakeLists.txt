cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (countpages)

add_executable (${PROJECT_NAME} main.c)
target_link_libraries (${PROJECT_NAME} ${MuPDF_LIBRARIES})
