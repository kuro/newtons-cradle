find_path(CG_INCLUDE_DIR
    NAMES Cg/cg.h
    PATHS
    "C:/Program Files/NVIDIA Corporation/Cg/include"
    )

find_library(CG_LIBRARY
    NAMES Cg
    PATHS
    "C:/Program Files/NVIDIA Corporation/Cg/lib"
    )

find_library(CG_GL_LIBRARY
    NAMES CgGL
    PATHS
    "C:/Program Files/NVIDIA Corporation/Cg/lib"
    )

set(CG_LIBRARIES ${CG_LIBRARY} ${CG_GL_LIBRARY})

if (CG_LIBRARY AND CG_INCLUDE_DIR)
    set(CG_FOUND TRUE)
else ()
    set(CG_FOUND FALSE)
endif ()
