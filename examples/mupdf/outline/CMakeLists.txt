project (outline_c)

add_executable (${PROJECT_NAME} main.c)
target_link_libraries (${PROJECT_NAME} ${MuPDF_LIBRARIES})
