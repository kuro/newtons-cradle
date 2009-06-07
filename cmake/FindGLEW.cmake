
find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h)

find_library(GLEW_LIBRARY NAMES glew)

if (GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
    set(GLEW_FOUND TRUE)
else ()
    set(GLEW_FOUND FALSE)
endif ()
