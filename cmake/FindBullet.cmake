find_path(BULLET_INCLUDE_DIR
    NAMES btBulletDynamicsCommon.h
    PATHS
    /usr/include/bullet
    )

find_library(BULLET_DYNAMICS_LIBRARY NAMES bulletdynamics)
find_library(BULLET_COLLISION_LIBRARY NAMES bulletcollision)
find_library(BULLET_MATH_LIBRARY NAMES bulletmath LinearMath)

set(BULLET_LIBRARIES
    ${BULLET_DYNAMICS_LIBRARY}
    ${BULLET_COLLISION_LIBRARY}
    ${BULLET_MATH_LIBRARY}
    )

if (BULLET_DYNAMICS_LIBRARY
        AND BULLET_COLLISION_LIBRARY
        AND BULLET_MATH_LIBRARY
        AND BULLET_INCLUDE_DIR
        )
    set(BULLET_FOUND TRUE)
else ()
    set(BULLET_FOUND FALSE)
endif ()
