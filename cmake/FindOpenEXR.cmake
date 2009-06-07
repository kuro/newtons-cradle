
find_path(OPENEXR_INCLUDE_DIR
    NAMES ImfRgba.h
    PATHS
    /usr/include/OpenEXR
    )
 
# from ilmbase
find_library(ILM_HALF_LIBRARY NAMES Half)
find_library(ILM_IEX_LIBRARY NAMES Iex)
find_library(ILM_THREAD_LIBRARY NAMES IlmThread)
find_library(ILM_IMATH_LIBRARY NAMES Imath)
 
# from openexr
find_library(ILM_IMF_LIBRARY NAMES IlmImf)
 
set(OPENEXR_LIBRARIES
    ${ILM_HALF_LIBRARY}
    ${ILM_IEX_LIBRARY}
    ${ILM_THREAD_LIBRARY}
    ${ILM_IMATH_LIBRARY}
    ${ILM_IMF_LIBRARY}
    )
 
if (ILM_IMF_LIBRARY AND OPENEXR_INCLUDE_DIR)
    set(OPENEXR_FOUND TRUE)
else ()
    set(OPENEXR_FOUND FALSE)
endif ()
